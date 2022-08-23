#include <Arduino.h>
#include "duty100.h"
#include "globals.h"
#include "receiver.h"

uint16_t duty100SecCounter = 0;

void duty100()
{
  // Check ambient light; adjust backlight
  currBrightness = analogRead(PHOTO_RESISTOR_PIN);
  // TODO: Adjust backlight
  // float val = 1023.0F * (sin(cnt) + 1) / 2;
  // analogWrite(TX, (int16_t)val);

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
  instrument.update(stgs.altitude, currTemp, currHumi, currPres);

  // Ping predictor to update its predictions
  predictor.update(currTemp, currHumi, currPres);

  // Receive radio
  receiveRadio(currExTemp, currExBattery);
}
