#ifndef CANVAS_H
#define CANVAS_H

#define N_ROWS 64
#define N_COLS 128

struct Font;

struct Canvas
{
  enum Fonts
  {
    font5,
    fontHumi,
    font10,
    font14,
    font30,
    wicons18,
  };

  bool dirty = true;
  uint8_t pixels[128 * 8];

  Canvas();
  void clear();

  // Draws text in 6x8 fixed font, aligned to 8-pixel line.
  void fwText(uint8_t x, uint8_t line, const char *str);

  // Draws text in dynamic font, anywhere. Coordinates are bottom left corner.
  void text(uint8_t x, uint8_t y, Fonts font, bool clearBg, const char *str);

  uint8_t measureText(Fonts font, const char *str);

  void setPixel(uint8_t x, uint8_t y, bool on);
  
  void line(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2);

  void box(uint8_t x, uint8_t y, uint8_t w, uint8_t h, bool on);

private:
  static Font *getFont(Fonts font);
};

#endif
