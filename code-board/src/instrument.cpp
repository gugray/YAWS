#include <Arduino.h>
#include "instrument.h"
#include <Adafruit_BME280.h>

Instrument::Instrument()
{
  bme = new Adafruit_BME280();
}

bool Instrument::begin(uint8_t bmeAddr)
{
  if (!bme->begin(bmeAddr))
    return false;
  bme->setSampling(Adafruit_BME280::MODE_NORMAL,
    Adafruit_BME280::SAMPLING_X8,
    Adafruit_BME280::SAMPLING_X8,
    Adafruit_BME280::SAMPLING_X8,
    Adafruit_BME280::FILTER_X8,
    Adafruit_BME280::STANDBY_MS_500);
  return true;
}

void Instrument::update(float altitude, float &temp, float &humi, float &pres)
{
  float xtemp = bme->readTemperature();
  float xhumi = bme->readHumidity();
  float xpres = bme->readPressure();
  xpres = xpres / pow((1 - altitude / 44330), 5.255) / 100;

  // We keep 6 readings. Public values are calculated by
  // throwing away highest and lowest, and taking the average of the rest.
  storeValues(xtemp, xhumi, xpres);
  temp = getFilteredReading(temps);
  humi = getFilteredReading(humis);
  pres = getFilteredReading(press);
}

void Instrument::storeValues(float temp, float humi, float pres)
{
  if (nReadings == MAX_INSTRUMENT_READINGS)
  {
    for (uint8_t i = 1; i < nReadings; ++i)
    {
      temps[i - 1] = temps[i];
      humis[i - 1] = humis[i];
      press[i - 1] = press[i];
    }
    temps[nReadings - 1] = temp;
    humis[nReadings - 1] = humi;
    press[nReadings - 1] = pres;
  }
  else
  {
    temps[nReadings] = temp;
    humis[nReadings] = humi;
    press[nReadings] = pres;
    ++nReadings;
  }
}

float Instrument::getFilteredReading(float *vals)
{
  if (nReadings == 0)
    return 0; // Should never happen

  // No full set of readings yet: just take latest
  if (nReadings < MAX_INSTRUMENT_READINGS)
    return vals[nReadings - 1];

  // Take average of readings without smallest and largest value
  float min = vals[0];
  float max = vals[0];
  float sum = vals[0];
  for (uint8_t i = 1; i < nReadings; ++i)
  {
    if (vals[i] < min)
      min = vals[i];
    if (vals[i] > max)
      max = vals[i];
    sum += vals[i];
  }
  return (sum - min - max) / (nReadings - 2);
}
