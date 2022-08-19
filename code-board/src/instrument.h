#ifndef INSTRUMENT_H
#define INSTRUMENT_H

class Adafruit_BME280;

class Instrument
{
private:
  Adafruit_BME280 *bme = 0;

public:
  Instrument();
  bool begin(uint8_t bmeAddr);
  void update(float altitude, float &temp, float &humi, float &pres);
};

#endif
