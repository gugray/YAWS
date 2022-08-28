#include <Arduino.h>
#include "weather_loop.h"
#include "globals.h"

void drawNotch(float angle)
{
  float r1 = 52;
  float r2 = 55;
  float x1 = 39 + r1 * sin(angle);
  float y1 = 87 - r1 * cos(angle);
  float x2 = 39 + r2 * sin(angle);
  float y2 = 87 - r2 * cos(angle);
  canvas.line(round(x1), round(y1), round(x2), round(y2));
}

void drawArrow(float angle)
{
  float cx = 39;
  float cy = 87;
  float r = 52;

  float x = 39 + r * sin(angle);
  float y = 87 - r * cos(angle);
  canvas.line(cx, cy, round(x), round(y));

  float arrowSideLen = 5;
  float arrowSideAngle = 0.45;
  float xa = x - arrowSideLen * sin(angle + arrowSideAngle);
  float ya = y + arrowSideLen * cos(angle + arrowSideAngle);
  float xb = x - arrowSideLen * sin(angle - arrowSideAngle);
  float yb = y + arrowSideLen * cos(angle - arrowSideAngle);
  for (float i = 0; i <= 5; ++i)
  {
    float xlerp = xa + (xb - xa) / 5 * i;
    float ylerp = ya + (yb - ya) / 5 * i;
    canvas.line(round(xlerp), round(ylerp), round(x), round(y));
  }
}

void drawInstrument(float pres)
{
  for (uint8_t x = 2; x <= 76; ++x)
  {
    uint8 y = (uint8_t)round(87 - sqrt(2809 - (39 - (float)x) * (39 - (float)x)));
    canvas.setPixel(x, y, true);
  }
  for (float ofs = -3; ofs <= 3; ++ofs)
  {
    float angle = ofs * PI / 12;
    drawNotch(angle);
  }
  float arrowAngle = (pres - 1010) / 10 * PI / 12;
  if (arrowAngle < -3 * PI / 12)
    arrowAngle = -3 * PI / 12;
  else if (arrowAngle > 3 * PI / 12)
    arrowAngle = 3 * PI / 12;
  drawArrow(arrowAngle);

  // Format pressure as number
  sprintf(buf, "%4.0fh", round(currPres));
  uint16_t presWidth = canvas.measureText(Canvas::font10, buf);

  // Clear out text area + border
  canvas.box(0, 57, 80, 7, false);
  canvas.box(37 - presWidth / 2, 49, presWidth + 2, 12, false);

  // Pressure as number
  canvas.text(38 - presWidth / 2, 61, Canvas::font10, true, buf);

  // Legend at edge of instrument
  canvas.text(0, 63, Canvas::font5, true, "980");
  canvas.text(64, 63, Canvas::font5, true, "1040");
}

char cstatus = 'a';

uint16_t weatherLoop()
{
  canvas.clear();

  // Write temperature in big digits
  sprintf(buf, "%5.1f*", currTemp);
  canvas.text(0, 30, Canvas::font30, true, buf);

  // Write secondary temperature
  // sprintf(buf, "%5.1f*", -currTemp);
  strcpy(buf, "--.-*");
  uint8_t tempWidth = canvas.measureText(Canvas::font14, buf);
  canvas.text(127 - tempWidth, 42, Canvas::font14, false, buf);

  sprintf(buf, "%d%%", (uint16_t)round(currHumi));
  uint16_t humiWidth = canvas.measureText(Canvas::font10, buf);
  canvas.text(129 - humiWidth, 10, Canvas::font10, true, buf);
  strcpy(buf, "00000");
  for (uint8_t i = 0; i < 5; ++i)
  {
    if (round(currHumi) >= 32.0 + i * 8)
      buf[i] = '1';
  }
  canvas.text(98, 20, Canvas::fontHumi, true, buf);

  drawInstrument(round(currPres));

  // auto trend = predictor.getTrend();
  // const char *trendStr = "T?";
  // if (trend == Predictor::trendRapidRise)
  //   trendStr = "Rise^Fast";
  // else if (trend == Predictor::trendRise)
  //   trendStr = "Rise";
  // else if (trend == Predictor::trendFlat)
  //   trendStr = "Stable";
  // else if (trend == Predictor::trendSink)
  //   trendStr = "Sink";
  // else if (trend == Predictor::trendRapidSink)
  //   trendStr = "Sink^Fast";
  // canvas.fwText(92, 6, trendStr);

  strcpy(buf, "x");
  auto state = predictor.getState();
  if (state == Predictor::stateUnstableUp)
    buf[0] = 'i';
  else if (state == Predictor::stateSun)
    buf[0] = 'd';
  else if (state == Predictor::stateMixed)
    buf[0] = 'f';
  else if (state == Predictor::stateCloud)
    buf[0] = 'b';
  else if (state == Predictor::stateUnstableDown)
    buf[0] = 'h';
  canvas.text(105, 63, Canvas::wicons18, true, buf);

  flushCanvasToDisplay();

  return 500;
}
