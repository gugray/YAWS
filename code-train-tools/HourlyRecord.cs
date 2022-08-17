using System;

class HourlyRecord
{
    /// <summary>
    /// Date and time (hour) of observation
    /// </summary>
    public DateTime Time;

    /// <summary>
    /// Precipitation mm
    /// </summary>
    public float Precip;

    /// <summary>
    /// Temperature C
    /// </summary>
    public float Temp;

    /// <summary>
    /// Sea level pressure hPa
    /// </summary>
    public float Pres;

    /// <summary>
    /// Relative humidity (%)
    /// </summary>
    public int RelHum;

    /// <summary>
    /// Cloudiness. -1 means N/A.
    /// 0: none; 1: little; 2: medium; 3: full
    /// </summary>
    public int Cloud;

    public int OrigWeatherCode;

    public override string ToString()
    {
        string res = Time.Year.ToString("D4") + "-" + Time.Month.ToString("D2") + "-" + Time.Day.ToString("D2") +
            "!" + Time.Hour.ToString("D2");
        res += '\t' + Precip.ToString("0.0");
        res += '\t' + Temp.ToString("0.0");
        res += '\t' + Pres.ToString("0.0");
        res += '\t' + RelHum.ToString();
        res += '\t' + Cloud.ToString();
        res += '\t' + OrigWeatherCode.ToString();
        return res;
    }

    public static HourlyRecord Parse(string line)
    {
        var parts = line.Split('\t');
        var rec = new HourlyRecord();
        rec.Time = new DateTime(
            int.Parse(parts[0].Substring(0, 4)),
            int.Parse(parts[0].Substring(5, 2)),
            int.Parse(parts[0].Substring(8, 2)),
            int.Parse(parts[0].Substring(11, 2)),
            0, 0);
        rec.Precip = float.Parse(parts[1]);
        rec.Temp = float.Parse(parts[2]);
        rec.Pres = float.Parse(parts[3]);
        rec.RelHum = int.Parse(parts[4]);
        rec.Cloud = int.Parse(parts[5]);
        rec.OrigWeatherCode = int.Parse(parts[6]);
        return rec;
    }
}
