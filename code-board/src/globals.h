#ifndef GLOBALS_H
#define GLOBALS_H

#include <Arduino.h>
#include <DOG7565R.h>
#include <WiFiClientSecureBearSSL.h>
#include <RH_ASK.h>
#include "settings.h"
#include "instrument.h"
#include "predictor.h"
#include "canvas.h"


#define LED_PIN LED_BUILTIN
#define RADIO_RX_PIN D6
#define RADIO_TX_PIN D4 // Not used; we're only receiving
#define BRIGHTNESS_PIN A0
#define DOG_CS_PIN D8
#define DOG_A0_PIN D3
#define DOG_RESET_PIN D0

#define BME_ADDR 0x76

extern RH_ASK radio;
extern DOG7565R dog;
extern BearSSL::WiFiClientSecure secureBearClient;

extern Settings stgs;
extern Instrument instrument;
extern Canvas canvas;
extern Predictor predictor;

extern float currTemp;
extern float currHumi;
extern float currPres;


#endif

