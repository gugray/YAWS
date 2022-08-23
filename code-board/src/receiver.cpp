#include <Arduino.h>
#include "receiver.h"
#include "globals.h"

int16_t count = 0;
const uint8_t radioBufSize = 128;
uint8_t radioBuf[radioBufSize];

bool receiveRadio(float &temp, float &battery)
{
  // uint8_t len = radioBufSize;
  // if (radio.recv(radioBuf, &len))
  // {
  //   Serial.printf("Messge %d received: '", count);
  //   for (uint8_t i = 0; i < len; ++i)
  //     Serial.print(radioBuf[i]);
  //   Serial.printf("'\n");
  //   ++count;
  // }
  return false;
}
