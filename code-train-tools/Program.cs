using System;
using System.Collections.Generic;

static partial class Program
{
    static void extractModelDataHu()
    {
        List<ModelRecord> train = new();
        List<ModelRecord> test = new();
        seriesToModel("weather-series-norm/hu-44527-budapest.txt", train, test);
        seriesToModel("weather-series-norm/hu-15310-szombathely.txt", train, test);

        int oldestPast = 6;
        int latestPast = 0;
        writeModelData("../code-train/hu-train.json", oldestPast, latestPast, train);
        writeModelData("../code-train/hu-test.json", oldestPast, latestPast, test);
    }

    static void Main(string[] args)
    {
        // Convert raw Hungarian data into our normalized series format
        convertHuRaw();

        // Extract training and test sets from normalized Hungarian series
        extractModelDataHu();
    }
}
