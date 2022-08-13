using System;
using System.Collections.Generic;
using System.IO;
using System.Reflection;
using Newtonsoft.Json;
using Newtonsoft.Json.Serialization;

partial class Program
{
    const float mediumRainThreshold = 1;
    const float heavyRainThreshold = 4;

    static void writeSeries(string fn, IEnumerable<HourlyRecord> series)
    {
        using StreamWriter sw = new(fn);
        foreach (var rec in series)
            sw.WriteLine(rec.ToString());
    }

    static float normPres(float pres)
    {
        if (pres < 950) pres = 950;
        if (pres > 1050) pres = 1050;
        return (pres - 950) / (1050 - 950);
    }

    static float normHum(float hum)
    {
        return hum / 100;
    }

    static float normTemp(float temp)
    {
        if (temp < -30) temp = -30;
        if (temp > 50) temp = 50;
        return (temp + 30) / (50 - -30);
    }

    static void seriesToModel(string fnSeries, List<ModelRecord> train, List<ModelRecord> test)
    {
        List<HourlyRecord> series = new();
        using StreamReader sr = new(fnSeries);
        string line;
        while ((line = sr.ReadLine()) != null)
            series.Add(HourlyRecord.Parse(line));
        series.Sort((a, b) => a.Time.CompareTo(b.Time));

        List<ModelRecord> mrecs = new();

        for (int i = 6; i < series.Count; ++i)
        {
            // Verify that it's a full series, at one-hour intervals
            bool sectionOk = true;
            for (int j = 6; j > 0; --j)
            {
                var rec1 = series[i - j];
                var rec2 = series[i - j + 1];
                if (rec2.Time.Subtract(rec1.Time).TotalHours != 1)
                    sectionOk = false;
            }
            if (series[i].Cloud == -1) sectionOk = false;
            if (!sectionOk)
                continue;

            // Create model record from section
            var mr = new ModelRecord();
            mr.Input.Pres6 = normPres(series[i - 6].Pres);
            mr.Input.RelHum6 = normHum(series[i - 6].RelHum);
            mr.Input.Temp6 = normTemp(series[i - 6].Temp);
            mr.Input.Pres5 = normPres(series[i - 5].Pres);
            mr.Input.RelHum5 = normHum(series[i - 5].RelHum);
            mr.Input.Temp5 = normTemp(series[i - 5].Temp);
            mr.Input.Pres4 = normPres(series[i - 4].Pres);
            mr.Input.RelHum4 = normHum(series[i - 4].RelHum);
            mr.Input.Temp4 = normTemp(series[i - 4].Temp);
            mr.Input.Pres3 = normPres(series[i - 3].Pres);
            mr.Input.RelHum3 = normHum(series[i - 3].RelHum);
            mr.Input.Temp3 = normTemp(series[i - 3].Temp);
            mr.Input.Pres2 = normPres(series[i - 2].Pres);
            mr.Input.RelHum2 = normHum(series[i - 2].RelHum);
            mr.Input.Temp2 = normTemp(series[i - 2].Temp);
            mr.Input.Pres1 = normPres(series[i - 1].Pres);
            mr.Input.RelHum1 = normHum(series[i - 1].RelHum);
            mr.Input.Temp1 = normTemp(series[i - 1].Temp);
            mr.Input.Pres0 = normPres(series[i].Pres);
            mr.Input.RelHum0 = normHum(series[i].RelHum);
            mr.Input.Temp0 = normTemp(series[i].Temp);

            // Encode discrete precipitation values
            var precip = series[i].Precip;
            if (precip == 0) mr.Output.Precip = "None";
            else if (precip < mediumRainThreshold) mr.Output.Precip = "Light";
            else if (precip < heavyRainThreshold) mr.Output.Precip = "Medium";
            else mr.Output.Precip = "Heavy";

            // Encode discrete cloud values
            var cloud = series[i].Cloud;
            if (cloud == 0) mr.Output.Cloud = "None";
            else if (cloud == 1) mr.Output.Cloud = "Light";
            else if (cloud == 2) mr.Output.Cloud = "Medium";
            else mr.Output.Cloud = "Full";

            // File away this record
            mrecs.Add(mr);
        }

        // Shuffle list of model records (Fisher-Yates algo)
        var rng = new Random(0);
        int n = mrecs.Count;
        while (n > 1)
        {
            n--;
            int k = rng.Next(n + 1);
            var value = mrecs[k];
            mrecs[k] = mrecs[n];
            mrecs[n] = value;
        }

        // 1:9 split into trainint and test set
        int trainSize = mrecs.Count * 9 / 10;
        int ix = 0;
        for (ix = 0; ix < trainSize; ++ix)
            train.Add(mrecs[ix]);
        for (; ix < mrecs.Count; ++ix)
            test.Add(mrecs[ix]);
    }

    class JsonContractResolver : DefaultContractResolver
    {
        readonly int oldestPast;
        readonly int latestPast;

        public JsonContractResolver(int oldestPast, int latestPast)
        {
            NamingStrategy = new CamelCaseNamingStrategy();
            if (oldestPast > 6 || oldestPast < 0) throw new ArgumentException(nameof(oldestPast));
            if (latestPast > 6 || latestPast < 0) throw new ArgumentException(nameof(latestPast));
            if (latestPast > oldestPast) throw new ArgumentException("Latest path must not be older that oldest past");
            this.oldestPast = oldestPast;
            this.latestPast = latestPast;
        }

        protected override JsonProperty CreateProperty(MemberInfo member, MemberSerialization memberSerialization)
        {
            var property = base.CreateProperty(member, memberSerialization);
            var pname = property.PropertyName;
            bool ignored = false;

            int endsWithNum = -1;
            if (int.TryParse(pname.Substring(pname.Length - 1, 1), out endsWithNum))
            {
                if (endsWithNum > oldestPast || endsWithNum < latestPast)
                    ignored = true;
            }

            if (ignored)
            {
                property.ShouldSerialize = i => false;
                property.Ignored = true;
            }

            return property;
        }
    }


    static void writeModelData(string fn, int oldestPast, int latestPast,
        List<ModelRecord> recs)
    {
        using StreamWriter sw = new(fn);

        var stgs = new JsonSerializerSettings();
        stgs.Formatting = Formatting.None;
        stgs.ContractResolver = new JsonContractResolver(oldestPast, latestPast);

        sw.WriteLine("[");
        for (int i = 0; i < recs.Count; ++i)
        {
            var rec = recs[i];
            string json = JsonConvert.SerializeObject(rec, stgs);
            sw.Write("  ");
            sw.Write(json);
            if (i != recs.Count - 1) sw.Write(",");
            sw.WriteLine();
        }
        sw.WriteLine("]");
    }
}
