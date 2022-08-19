#include <Arduino.h>
#include "instrument.h"
#include <Adafruit_BME280.h>


Instrument::Instrument()
{
  bme = new Adafruit_BME280();
}

bool Instrument::begin(uint8_t bmeAddr)
{
  return bme->begin(bmeAddr);
}

void Instrument::update(float altitude, float &temp, float &humi, float &pres)
{
  temp = bme->readTemperature();
  humi = bme->readHumidity();
  pres = bme->readPressure();
  pres = pres / pow((1 - altitude / 44330), 5.255) / 100;
}
