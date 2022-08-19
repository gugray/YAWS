#include <Arduino.h>
#include "predictor.h"

#define HPA_HI 1022.6
#define HPA_LO 1009.1
#define RISE_QUICK 0.75
#define RISE_MEDIUM 0.42
#define RISE_SLOW 0.25
#define SINK_QUICK -0.75
#define SINK_MEDIUM -0.42
#define SINK_SLOW -0.25

void Predictor::update(float temp, float humi, float pres)
{
  // We record readings every 10 minutes
  if (seconds != 0)
  {
    seconds++;
    if (seconds == 600)
      seconds = 0;
    return;
  }
  seconds++;

  // File new readings
  if (nReadings < MAX_PREDICTOR_READINGS)
  {
    readings[nReadings] = Reading{temp, humi, pres};
    ++nReadings;
  }
  else
  {
    for (uint16 i = 1; i < MAX_PREDICTOR_READINGS; ++i)
      readings[i - 1] = readings[i];
    readings[MAX_PREDICTOR_READINGS - 1] = Reading{temp, humi, pres};
  }

  // Update state and trend
  predict();
}

void Predictor::predict()
{
  // Not enough history? Just go by absolute pressure
  float current = readings[nReadings - 1].pres;
  if (nReadings < 7)
  {
    if (current >= HPA_HI)
      state = stateSun;
    else if (current >= HPA_LO)
      state = stateMixed;
    else
      state = stateCloud;
    trend = trendUnknown;
    return;
  }
  float delta = current - readings[nReadings - 6 - 1].pres;
  if (nReadings >= 19)
    delta = (current - readings[nReadings - 18 - 1].pres) / 3;

  if (delta > RISE_QUICK)
  {
    // Rapid rise: Unstable
    trend = trendRapidRise;
    state = stateUnstableUp;
  }
  else if (delta > RISE_MEDIUM)
  {
    // Moderate rise: good weather ahead
    trend = trendRise;
    state = stateSun;
  }
  else if (delta > RISE_SLOW)
  {
    // Slow rise: improving weather
    trend = trendRise;
    state = improveState(state);
    // If we're in low-pressure range, don't get better then mixed
    if (current < HPA_LO && state == stateSun)
      state = stateMixed;
  }
  else if (delta < SINK_QUICK)
  {
    // Quickly sinking: Stormy weather
    trend = trendRapidSink;
    state = stateUnstableDown;
  }
  else if (delta < SINK_MEDIUM)
  {
    // Moderately sinking: bad weather ahead
    trend = trendSink;
    state = stateCloud;
  }
  else if (delta < SINK_SLOW)
  {
    trend = trendSink;
    state = deteriorateState(state);
    // If we're in high-pressure range, don't get worse than mixed
    if (current > HPA_HI && state == stateCloud)
      state = stateMixed;
  }
  else
  {
    // Absolute rate of change very small in either direction: weather is steady
    trend = trendFlat;
    // Remove extremes, just bad or good weather
    if (state == stateUnstableDown)
      state = stateCloud;
    else if (state == stateUnstableUp)
      state = stateSun;
  }
}

Predictor::State Predictor::improveState(State state)
{
  if (state == stateUnstableUp)
    return stateSun;
  else if (state == stateUnstableDown)
    return stateCloud;
  else if (state == stateCloud)
    return stateMixed;
  else if (state == stateMixed)
    return stateSun;
  else if (state == stateSun)
    return stateSun;
  // Unknown should not happen though
  return stateUnknown;
}

Predictor::State Predictor::deteriorateState(State state)
{
  if (state == stateUnstableUp)
    return stateMixed;
  else if (state == stateUnstableDown)
    return stateCloud;
  else if (state == stateCloud)
    return stateCloud;
  else if (state == stateMixed)
    return stateCloud;
  else if (state == stateSun)
    return stateMixed;
  // Unknown should not happen though
  return stateUnknown;
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
