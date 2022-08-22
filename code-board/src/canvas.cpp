#include <Arduino.h>
#include <avr/pgmspace.h>
#include <font_6x8.h>
#include "canvas.h"
#include "canvas_font_data.h"

struct Font
{
	uint8_t height;
	uint8_t nGlyphs;
	Glyph *glyphs;
	uint8_t *pixelData;

	Glyph *getGlyph(char c)
	{
		for (uint8_t ix = 0; ix < nGlyphs; ++ix)
		{
			if (glyphs[ix].code == c)
				return &glyphs[ix];
		}
		return 0;
	}
};

Font font5Data = {
		5,
		font5_GLYPH_COUNT,
		font5Glyphs,
		font5PixelData,
};

Font fontHumiData = {
		7,
		fontHumi_GLYPH_COUNT,
		fontHumiGlyphs,
		fontHumiPixelData,
};

Font font10Data = {
		10,
		font10_GLYPH_COUNT,
		font10Glyphs,
		font10PixelData,
};

Font font16Data = {
		16,
		font16_GLYPH_COUNT,
		font16Glyphs,
		font16PixelData,
};

Font font30Data = {
		30,
		font30_GLYPH_COUNT,
		font30Glyphs,
		font30PixelData,
};

Canvas::Canvas()
{
	clear();
}

void Canvas::clear()
{
	dirty = true;
	uint16_t i = 0;
	for (uint8_t p = 0; p < N_ROWS / 8; ++p)
	{
		for (uint8_t x = 0; x < N_COLS; ++x, ++i)
		{
			pixels[i] = 0;
		}
	}
}

void Canvas::setPixel(uint8_t x, uint8_t y, bool on)
{
	if (x >= 128 || y >= 64)
		return;
	dirty = true;
	uint16_t row = y / 8;
	uint8_t bit = y % 8;
	if (on)
		pixels[row * N_COLS + x] |= (1 << bit);
	else
		pixels[row * N_COLS + x] &= ~(1 << bit);
}

void Canvas::line(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2)
{
	float dx = x2 - x1;
	float dy = y2 - y1;
	float len = round(sqrt(dx * dx + dy * dy));
	for (float i = 0; i <= len; i += 1)
	{
		float x = x1 + dx * i / len;
		float y = y1 + dy * i / len;
		setPixel(round(x), round(y), true);
	}
}

void Canvas::box(uint8_t x, uint8_t y, uint8_t w, uint8_t h, bool on)
{
	dirty = true;
	for (uint8_t i = x; i < x + w; ++i)
	{
		for (uint8_t j = y; j < y + h; ++j)
		{
			if (i >= 128 || j >= 64)
				continue;
			setPixel(i, j, on);
		}
	}
}

Font *Canvas::getFont(Fonts font)
{
	Font *fd = 0;
	if (font == font5)
		fd = &font5Data;
	else if (font == fontHumi)
		fd = &fontHumiData;
	else if (font == font10)
		fd = &font10Data;
	else if (font == font16)
		fd = &font16Data;
	else if (font == font30)
		fd = &font30Data;
	return fd;
}

uint8_t Canvas::measureText(Fonts font, const char *str)
{
	Font *fd = getFont(font);
	uint8_t width = 0;
	for (uint8_t ix = 0; ix < strlen(str); ++ix)
	{
		Glyph *g = fd->getGlyph(str[ix]);
		if (g == 0)
			continue;
		width += g->advance;
	}
	return width;
}

void Canvas::text(uint8_t x, uint8_t y, Fonts font, bool clearBg, const char *str)
{
	Font *fd = getFont(font);
	for (uint8_t ix = 0; ix < strlen(str) && x < 128; ++ix)
	{
		Glyph *g = fd->getGlyph(str[ix]);
		if (g == 0)
			continue;
		uint16_t pdataPos = g->pixelDataIx;
		for (uint8_t chrx = 0; chrx < g->advance && x < 128; ++chrx)
		{
			for (uint8_t chry = 0; chry < fd->height; ++chry)
			{
				uint8_t byteIx = chry / 8;
				uint8_t bitIx = chry % 8;
				uint8_t mask = 1 << bitIx;
				uint8_t *pd = fd->pixelData + pdataPos + byteIx;
				bool isOn = (*pd & mask) != 0;
				if (!clearBg && !isOn)
					continue;
				setPixel(x, y + chry - fd->height, isOn);
			}
			pdataPos += ((fd->height - 1) / 8) + 1;
			++x;
		}
	}
}

void Canvas::fwText(uint8_t x, uint8_t line, const char *str)
{
	dirty = true;
	const byte *fontData = font_6x8;
	uint8_t firstCode = fontData[2]; // get first defined character
	uint8_t lastCode = fontData[3];	 // get last defined character
	uint8_t charWidth = 6;					 // width in pixel of one char

	uint16_t arrPos = line * N_COLS + x;
	for (; *str != 0; ++str)
	{
		uint8_t charVal = *str;
		if (charVal < firstCode || charVal > lastCode)
			continue;

		uint16_t fontDataPos = 8 + (uint16_t)(charVal - firstCode) * charWidth;

		uint8_t nCols = charWidth;
		if (x + charWidth > N_COLS) // stay inside display area
			nCols = N_COLS - x;

		for (uint8_t i = 0; i < nCols; i++, arrPos++)
		{
			pixels[arrPos] = fontData[fontDataPos + i];
		}
	}
}
