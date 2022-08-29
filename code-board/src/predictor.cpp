#include <Arduino.h>
#include "predictor.h"

#define HIGH_THRESHOLD 1022.6
#define LOW_THRESHOLD 1009.1
#define CHG_ULTRA 1.2
#define CHG_FAST 0.75
#define CHG_SLOW 0.42
#define CHG_THRESHOLD 0.25

void Predictor::update(float pres)
{
  // We record readings every 5 minutes
  if (seconds != 0)
  {
    seconds++;
    if (seconds == PREDICTOR_PERIOD_SECONDS)
      seconds = 0;
    return;
  }
  seconds++;

  // File new readings
  if (nReadings < MAX_PREDICTOR_READINGS)
  {
    readings[nReadings] = pres;
    ++nReadings;
  }
  else
  {
    for (uint16 i = 1; i < MAX_PREDICTOR_READINGS; ++i)
      readings[i - 1] = readings[i];
    readings[MAX_PREDICTOR_READINGS - 1] = pres;
  }

  // Update state and trend
  predict();
}

void Predictor::predict()
{
  // Less history that one full hour? Just go by absolute pressure
  float current = readings[nReadings - 1];
  if (nReadings < 13)
  {
    if (current >= HIGH_THRESHOLD)
      state = stateSun;
    else if (current >= LOW_THRESHOLD)
      state = stateMixed;
    else
      state = stateCloud;
    trend = trendUnknown;
    return;
  }

  // Get pressure trends from last hour, and from full history
  float fullSlope, hourSlope;
  getSlopes(fullSlope, hourSlope);

  // If we have very fasdt fall rise/fall in last hour, we go by that alone
  // Otherwisewe look at trend of full series (3 hours normally)
  if (fullSlope < -CHG_FAST || hourSlope < -CHG_ULTRA)
  {
    trend = trendRapidSink;
    state = stateStorm;
    return;
  }
  else if (fullSlope > CHG_FAST || hourSlope > CHG_ULTRA)
  {
    trend = trendRapidRise;
    state = stateUnstableRise;
    return;
  }
  else if (fullSlope < -CHG_SLOW)
  {
    trend = trendSink;
    state = stateCloud;
    return;

  }
  else if (fullSlope > CHG_SLOW)
  {
    trend = trendRise;
    state = stateSun;
    return;
  }
  else if (fullSlope < -CHG_THRESHOLD)
  {
    trend = trendSink;
    if (current > LOW_THRESHOLD)
      state = stateMixed;
  }
  else if (fullSlope > CHG_THRESHOLD)
  {
    trend = trendRise;
    if (current < HIGH_THRESHOLD)
      state = stateMixed;
  }
  else
  {
    // State doesn't change with stable trend
    trend = trendFlat;
  }
}

void Predictor::getSlopes(float &fullSlope, float &hourSlope)
{
  // Find the best-fit line
  // Every hour is an increase of 1 in x
  // Y values are the pressure readings
  // Resulting slope is the hourly rate of change
  // https://faculty.cs.niu.edu/~hutchins/csci297p2/webpages/best-fit.htm
  float sumx = 0,
        sumy = 0,
        sumx2 = 0,
        sumxy = 0;
  int16_t i;
  for (i = nReadings - 1; i >= nReadings - 13; --i)
  {
    float x = (float)i / 12;
    float y = readings[i];
    sumx += x;
    sumy += y;
    sumx2 += x * x;
    sumxy += x * y;
  }
  hourSlope = (sumxy - sumx * sumy / 13) / (sumx2 - sumx * sumx / 13);
  for (i = nReadings - 14; i >= 0; --i)
  {
    float x = (float)i / 12;
    float y = readings[i];
    sumx += x;
    sumy += y;
    sumx2 += x * x;
    sumxy += x * y;
  }
  fullSlope = (sumxy - sumx * sumy / nReadings) / (sumx2 - sumx * sumx / nReadings);
}

// # Use the calculated pressure difference to finally make the forecast
// # -------------------------------------------------------------------
// # Rising Conditions
// # -----------------
// # Quickly rising, very unstable weather condition
// if pressurerdiff > 0.75:
//     shutil.copyfile('/home/pi/pressure_info/Unstable.png', '/var/www/html/Forecast.png')
//     shutil.copyfile('/home/pi/pressure_info/Up.png', '/var/www/html/Arrow.png')
// # Slowly rising, good weather condition, tendency rising
// elif pressurerdiff > 0.42:
//     shutil.copyfile('/home/pi/pressure_info/Sun.png', '/var/www/html/Forecast.png')
//     shutil.copyfile('/home/pi/pressure_info/UpRight.png', '/var/www/html/Arrow.png')
// # Change in weather condition is possible, tendency rising
// elif pressurerdiff > 0.25:
//     shutil.copyfile('/home/pi/pressure_info/UpRight.png', '/var/www/html/Arrow.png')
//     if (currentpress >= 1006 and currentpress <= 1020) or currentpress < 1006:
//         shutil.copyfile('/home/pi/pressure_info/SunCloud.png', '/var/www/html/Forecast.png')
// # Falling Conditions
// # ------------------
// # Quickly falling, thunderstorm is highly possible
// elif pressurerdiff < -0.75:
//     shutil.copyfile('/home/pi/pressure_info/Storm.png', '/var/www/html/Forecast.png')
//     shutil.copyfile('/home/pi/pressure_info/Down.png', '/var/www/html/Arrow.png')
// # Slowly falling, rainy weather condition, tendency falling
// elif pressurerdiff < -0.42:
//     shutil.copyfile('/home/pi/pressure_info/Rain.png', '/var/www/html/Forecast.png')
//     shutil.copyfile('/home/pi/pressure_info/DownRight.png', '/var/www/html/Arrow.png')
// # Condition change is possible, tendency falling
// elif pressurerdiff < -0.25:
//     shutil.copyfile(’/home/pi/pressure _info/DownRight.png’, ’/var/www/html/Arrow.png’)
//     if (currentpress >= 1006 and currentpress <= 1020) or currentpress > 1020:
//         shutil.copyfile(’/home/pi/pressure _info/SunCloud.png’, ’/var/www/html/Forecast.png’)
// # Steady Conditions
// # -----------------
// # Condition is stable, don't change the weather symbol (sun, rain or sun/cloud), just change the arrow
// elif pressurerdiff <= 0.25 and pressurerdiff >= -0.25:
//     shutil.copyfile('/home/pi/pressure_info/Right.png', '/var/www/html/Arrow.png')
