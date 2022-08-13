class ModelInput
{
    public float Pres6;
    public float RelHum6;
    public float Temp6;

    public float Pres5;
    public float RelHum5;
    public float Temp5;

    public float Pres4;
    public float RelHum4;
    public float Temp4;

    public float Pres3;
    public float RelHum3;
    public float Temp3;

    public float Pres2;
    public float RelHum2;
    public float Temp2;

    public float Pres1;
    public float RelHum1;
    public float Temp1;

    public float Pres0;
    public float RelHum0;
    public float Temp0;
}

class ModelOutput
{
    public string Precip;
    public string Cloud;
}

class ModelRecord
{
    public ModelInput Input = new ModelInput();
    public ModelOutput Output = new ModelOutput();
}
