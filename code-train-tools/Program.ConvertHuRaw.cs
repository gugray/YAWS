using System;
using System.Collections.Generic;
using System.IO;

partial class Program
{
    static void convertHuRaw()
    {
        List<HourlyRecord> series;

        // Convert series for station 15310 (Szombathely)
        series = new();
        series.AddRange(readHuRawFile("weather-hu/HABP_1H_20020227_20211231_15310.csv"));
        series.AddRange(readHuRawFile("weather-hu/HABP_1H_20220101_20220810_15310.csv"));
        writeSeries("weather-series-norm/hu-15310-szombathely.txt", series);

        // Convert series for station 44527 (Budapest-Pestszentlőrinc)
        series = new();
        series.AddRange(readHuRawFile("weather-hu/HABP_1H_20020101_20211231_44527.csv"));
        series.AddRange(readHuRawFile("weather-hu/HABP_1H_20220101_20220810_44527.csv"));
        writeSeries("weather-series-norm/hu-44527-budapest.txt", series);
    }

    static List<HourlyRecord> readHuRawFile(string fn)
    {
        List<HourlyRecord> res = new();
        using StreamReader sr = new(fn);
        string line;
        while ((line = sr.ReadLine()) != null)
        {
            if (line.StartsWith("#") || line == "") continue;
            var parts = line.Split(';');
            for (int i = 0; i < parts.Length; ++i)
                parts[i] = parts[i].Trim();
            if (parts[0] == "StationNumber") continue;

            var rec = new HourlyRecord();
            rec.Time = new DateTime(
                    int.Parse(parts[1].Substring(0, 4)),
                    int.Parse(parts[1].Substring(4, 2)),
                    int.Parse(parts[1].Substring(6, 2)),
                    int.Parse(parts[1].Substring(8, 2)),
                    0, 0);
            rec.Precip = float.Parse(parts[2]);
            rec.Temp = float.Parse(parts[4]);
            rec.Pres = float.Parse(parts[36]);
            rec.RelHum = (int)Math.Round(float.Parse(parts[16]));
            rec.Cloud = convertHuCloud(int.Parse(parts[34]));
            rec.OrigWeatherCode = int.Parse(parts[34]);

            if (rec.Precip == -999) continue;
            if (rec.Temp == -999) continue;
            if (rec.Pres == -999) continue;
            if (rec.RelHum == -999) continue;

            res.Add(rec);
        }
        return res;
    }

    static int convertHuCloud(int val)
    {
        // -999: Not coded
        if (val == -999) return -1;

        // 1 derült
        else if (val == 1) return 0;

        // 2 kissé felhős
        else if (val == 2) return 1;

        // 3 közepesen felhős
        else if (val == 3) return 2;

        // 4 erősen felhős
        else if (val == 4) return 2;

        // 5 borult
        else if (val == 5) return 3;

        // 6 fátyolfelhős
        else if (val == 6) return 1;

        // 7 ködös
        else if (val == 7) return 3;

        // 9 derült, párás
        else if (val == 9) return 0;

        // 10 közepesen felhős, párás
        else if (val == 10) return 2;

        // 11 borult, párás
        else if (val == 11) return 3;

        // 12 erősen fátyolfelhős
        else if (val == 12) return 2;

        // 101 szitálás
        // 102 eső
        // 103 zápor
        // 104 zivatar esővel
        // 105 ónos szitálás
        // 106 ónos eső
        // 107 hószállingózás
        // 108 havazás
        // 109 hózápor
        // 110 havaseső
        // 112 hózivatar
        else if (val >= 101 && val <= 112) return 3;

        // 202 erős eső
        // 203 erős zápor
        // 208 erős havazás
        // 209 erős hózápor
        else if (val >= 202 && val <= 209) return 3;

        // 304 zivatar záporral
        // 310 havaseső zápor
        else if (val == 304 || val == 310) return 3;

        // 500 hófúvás
        // 600 jégeső
        // 601 dörgés
        else if (val == 500 || val == 600 || val == 601) return 3;

        // Unexpected value
        else throw new Exception("Undocumented weather value: " + val);
    }
}
