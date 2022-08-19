#include <Arduino.h>
#include "duty100.h"
#include "globals.h"

uint16_t duty100SecCounter = 0;

void duty100()
{
  if (duty100SecCounter != 0)
  {
    duty100SecCounter++;
    if (duty100SecCounter == 10)
      duty100SecCounter = 0;
    return;
  }
  duty100SecCounter++;

  // Once per second, update readings
  instrument.update(stgs.altitude, currTemp, currHumi, currPres);

  // Also once per second, ping predictor
  predictor.update(currTemp, currHumi, currPres);
}
