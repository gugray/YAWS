#include <Arduino.h>
#include "duty100.h"
#include "globals.h"
#include "config.h"
#include "receiver.h"

#define MIN_BACKLIGHT_DUTY 16
#define MAX_BACKLIGHT_DUTY 512
#define LIGHTSENSOR_N_VALUES 20
#define LIGHTSENSOR_DARK_THRESHOLD 100
#define LIGHTSENSOR_BRIGHT_THRESHOLD 700

uint32_t btnPressedAt = 0xffffffff;
uint16_t lightReadings[LIGHTSENSOR_N_VALUES];
uint8_t lightReadingIx = 0;
uint16_t lightReadingSum = 0;
uint16_t currBacklightDuty = 0;
uint16_t duty100SecCounter = 0;

void initDuty100()
{
  for (uint8_t i = 0; i < LIGHTSENSOR_N_VALUES; ++i)
    lightReadings[i] = 0;
}

void updateButton()
{
  // Is button pressed? LOW is pressed (has pullup)
  // Don't do any of this if button press has triggered some action already.
  auto btnVal = digitalRead(BUTTON_PIN);

  if (btnVal == LOW && buttonPressed != -1)
  {
    auto currMillis = millis() & 0x7fffffff;
    if (btnPressedAt == 0xffffffff)
    {
      buttonPressed = 1;
      btnPressedAt = currMillis;
    }
    else
    {
      buttonPressed = currMillis - btnPressedAt;
    }
  }
  // Button is not pressed
  else if (btnVal == HIGH)
  {
    btnPressedAt = 0xffffffff;
    buttonPressed = 0;
  }
}

void updateBacklight()
{
  // Update ring buffer of last N readings; get updated average
  lightReadingIx += 1;
  if (lightReadingIx == LIGHTSENSOR_N_VALUES)
    lightReadingIx = 0;
  lightReadingSum -= lightReadings[lightReadingIx];
  auto currLight = analogRead(PHOTO_RESISTOR_PIN);
  lightReadings[lightReadingIx] = currLight;
  lightReadingSum += currLight;
  auto light = lightReadingSum / LIGHTSENSOR_N_VALUES;

  // Decide what is our new blacklight duty cycle
  uint32_t newBLDuty = 0;

  if (light < LIGHTSENSOR_DARK_THRESHOLD)
    newBLDuty = MIN_BACKLIGHT_DUTY;
  else if (light > LIGHTSENSOR_BRIGHT_THRESHOLD)
    newBLDuty = MAX_BACKLIGHT_DUTY;
  else
  {
    newBLDuty = MIN_BACKLIGHT_DUTY +
                (light - LIGHTSENSOR_DARK_THRESHOLD) *
                    (MAX_BACKLIGHT_DUTY - MIN_BACKLIGHT_DUTY) /
                    (LIGHTSENSOR_BRIGHT_THRESHOLD - LIGHTSENSOR_DARK_THRESHOLD);
  }

  // No update if no change
  if (newBLDuty == currBacklightDuty)
    return;

  // Update PWM duty on backlight pin
  currBacklightDuty = (uint16_t)newBLDuty;
  analogWrite(BACKLIGHT_PIN, currBacklightDuty);
}

void duty100()
{
  // Setup mode push button - read pin and update state
  updateButton();

  // Check ambient light; adjust backlight
  updateBacklight();

  // // Anything on the radio? (from external sensor)
  // receiveRadio(currExTemp, currExBattery);

  // Count cycles for duties we do once per second
  if (duty100SecCounter != 0)
  {
    duty100SecCounter++;
    if (duty100SecCounter == 10)
      duty100SecCounter = 0;
    return;
  }
  duty100SecCounter++;

  // Code from here runs once per second

  // Update instrument readings
  instrument.update(Config::ofsPres, Config::ofsTemp, currTemp, currHumi, currPres);

  // Ping predictor to update its predictions
  predictor.update(currPres);

  // Receive radio
  receiveRadio(currExTemp, currExBattery);
}
