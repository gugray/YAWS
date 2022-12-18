#ifndef INSTRUMENT_H
#define INSTRUMENT_H

#define MAX_INSTRUMENT_READINGS 6

class Adafruit_BME280;

class Instrument
{
private:
  Adafruit_BME280 *bme = 0;
  uint8_t nReadings = 0;
  float temps[MAX_INSTRUMENT_READINGS];
  float humis[MAX_INSTRUMENT_READINGS];
  float press[MAX_INSTRUMENT_READINGS];

private:
  void storeValues(float temp, float humi, float pres);
  float getFilteredReading(float *vals);

public:
  Instrument();
  bool begin(uint8_t bmeAddr);
  void update(float ofsPres, float ofsTemp, float &temp, float &humi, float &pres);
};

#endif
