#include <Arduino.h>
#include <DOG7565R.h>
#include "canvas.h"

#define DOG_CS_PIN D8
#define DOG_A0_PIN D3
#define DOG_RESET_PIN D0

DOG7565R dog;
Canvas canvas;

void flushCanvasToDisplay()
{
  if (!canvas.dirty)
    return;
  canvas.dirty = false;
  uint16_t i = 0;
  for (uint8_t p = 0; p < N_ROWS / 8; ++p)
  {
    dog.position(0, p);
    for (uint8_t x = 0; x < N_COLS; ++x, ++i)
    {
      dog.data(canvas.pixels[i]);
    }
  }
}


void setup()
{
  pinMode(D1, OUTPUT);
  digitalWrite(D1, HIGH);
  delay(500);

  // analogWriteRange(1023);
  // analogWriteFreq(880);
  dog.initialize(DOG_CS_PIN, DOG_A0_PIN, DOG_RESET_PIN, DOGM128);
  canvas.clear();
  canvas.line(5, 5, 122, 5);
  canvas.fwText(20, 7, "Hello world!");
  flushCanvasToDisplay();

  digitalWrite(D1, LOW);
  delay(500);
}

void loop()
{
  digitalWrite(D1, HIGH);
  delay(500);
  digitalWrite(D1, LOW);
  delay(500);
}
