#include <Arduino.h>
#include "TinyRF_TX.h"

// https://lastminuteengineers.com/433mhz-rf-wireless-arduino-tutorial/
// https://www.airspayce.com/mikem/arduino/RadioHead/classRH__ASK.html

#define LED_PIN 13
#define RADIO_TX_PIN 2



void setup()
{
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, HIGH);
  delay(500);

  setupTransmitter();

  // pinMode(RADIO_TX_PIN, OUTPUT);
  // digitalWrite(RADIO_TX_PIN, HIGH);

  digitalWrite(LED_PIN, LOW);
  delay(500);
}

#pragma pack(push, 1)
struct Data
{
  uint8_t val1Hi = 0;
  uint8_t val1Lo = 0;
  uint8_t val2Hi = 0;
  uint8_t val2Lo = 0;
};
#pragma pack(pop)


uint8_t msg[16];
Data data;

void loop()
{
  // digitalWrite(RADIO_TX_PIN, HIGH);
  // delay(1);
  // digitalWrite(RADIO_TX_PIN, LOW);
  // delay(1);

  digitalWrite(LED_PIN, HIGH);
  delay(500);

  memcpy(msg, &data, sizeof(Data));
  send(msg, sizeof(Data));

  data.val1Lo += 1;
  if (data.val1Lo == 0) data.val1Hi += 1;
  data.val2Lo += 1;
  if (data.val2Lo == 0) data.val2Hi += 1;

  digitalWrite(LED_PIN, LOW);
  delay(500);
}
