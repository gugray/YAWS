#include <Arduino.h>
#include <RH_ASK.h>

// https://lastminuteengineers.com/433mhz-rf-wireless-arduino-tutorial/
// https://www.airspayce.com/mikem/arduino/RadioHead/classRH__ASK.html

#define LED_PIN 13
#define RADIO_RX_PIN 3
#define RADIO_TX_PIN 2

RH_ASK radio(2000, RADIO_RX_PIN, RADIO_TX_PIN);

const char *msg = "Hello World";

void setup()
{
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, HIGH);
  delay(500);

  radio.init();

  // pinMode(RADIO_TX_PIN, OUTPUT);
  // digitalWrite(RADIO_TX_PIN, HIGH);

  digitalWrite(LED_PIN, LOW);
  delay(500);
}

void loop()
{
  // digitalWrite(RADIO_TX_PIN, HIGH);
  // delay(1);
  // digitalWrite(RADIO_TX_PIN, LOW);
  // delay(1);

  digitalWrite(LED_PIN, HIGH);
  delay(500);

  radio.setModeTx();
  radio.send((uint8_t *)msg, strlen(msg));
  radio.waitPacketSent();
  radio.setModeIdle();

  digitalWrite(LED_PIN, LOW);
  delay(500);
}
