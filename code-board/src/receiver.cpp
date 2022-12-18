#include <Arduino.h>
#include "radio/TinyRF_RX.h"
#include "receiver.h"
#include "globals.h"

const uint8_t radioBufSize = 128;
uint8_t radioBuf[radioBufSize];

#pragma pack(push, 1)
struct Data
{
  uint8_t val1Hi = 0;
  uint8_t val1Lo = 0;
  uint8_t val2Hi = 0;
  uint8_t val2Lo = 0;
};
#pragma pack(pop)

Data data;

bool receiveRadio(float &temp, float &battery)
{
  bool readAny = false;
  while (true)
  {
    uint8_t cnt;
    getReceivedData(radioBuf, sizeof(Data), cnt);
    if (cnt == sizeof(Data))
    {
      memcpy(&data, radioBuf, sizeof(Data));
      readAny = true;
    }
    else
      break;
  }
  if (!readAny)
    return false;

  uint16_t iTemp = data.val1Lo + data.val1Hi * 256;
  uint16_t iBattery = data.val2Lo + data.val2Hi * 256;

  temp = ((float)iTemp - 2400.0F) / 80.0F;
  battery = (float)iBattery / 100.0F;

  return true;
}
