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

Font font14Data = {
		14,
		font14_GLYPH_COUNT,
		font14Glyphs,
		font14PixelData,
};

Font font30Data = {
		30,
		font30_GLYPH_COUNT,
		font30Glyphs,
		font30PixelData,
};

Font font30NData = {
		30,
		font30N_GLYPH_COUNT,
		font30NGlyphs,
		font30NPixelData,
};

Font wicons18Data = {
		18,
		wicons18_GLYPH_COUNT,
		wicons18Glyphs,
		wicons18PixelData,
};

Font arrows14Data = {
		14,
		arrows14_GLYPH_COUNT,
		arrows14Glyphs,
		arrows14PixelData,
};

// 23x23 blue noise
const uint16_t noiseSz = 23;
uint16_t blueNoise[noiseSz*noiseSz] = {
	71,293,428,73,234,377,444,17,302,162,421,252,435,152,278,126,291,454,355,93,501,81,344,
	518,169,339,32,187,124,243,223,364,58,279,100,326,504,397,527,43,182,9,319,394,208,430,
	254,412,114,390,473,332,492,110,181,40,526,37,308,385,459,338,84,160,262,468,123,34,275,
	402,283,211,227,54,83,448,348,352,26,95,142,416,199,193,224,170,446,106,270,50,367,186,
	153,307,503,7,374,196,399,465,204,269,438,245,55,481,472,362,15,140,330,388,158,298,494,
	102,486,131,456,328,259,3,144,313,149,510,190,425,380,375,498,217,2,513,231,423,67,242,
	201,65,296,249,470,477,323,104,118,218,347,257,51,167,91,266,6,148,341,464,523,282,410,
	508,173,356,89,408,165,419,260,496,48,432,301,476,64,335,112,451,220,21,370,174,304,59,
	515,136,317,265,407,452,62,240,14,288,178,351,132,417,519,77,238,197,511,45,479,74,321,
	79,228,20,334,38,311,214,121,461,155,361,285,107,212,315,176,443,248,98,400,383,25,433,
	440,191,381,135,489,522,31,371,392,86,272,11,359,490,404,28,146,485,129,290,116,235,205,
	99,340,447,524,42,305,369,24,207,322,171,185,268,449,108,316,429,244,442,115,436,206,387,
	180,161,277,222,378,119,109,250,401,437,56,22,358,133,215,393,46,179,497,366,53,280,143,
	488,363,4,418,85,239,299,120,502,466,247,505,336,76,487,251,154,16,309,172,521,343,69,
	475,295,200,331,141,450,391,458,192,130,349,297,117,457,420,41,405,509,94,306,324,480,87,
	156,357,33,406,10,90,145,18,281,75,411,471,5,57,163,350,225,229,398,413,0,462,354,
	427,66,258,213,271,512,495,314,60,226,520,233,198,376,286,516,88,30,256,125,274,147,189,
	232,493,455,253,105,175,463,267,241,127,431,68,484,320,122,517,255,273,134,500,312,82,474,
	346,47,166,499,202,506,36,209,478,13,103,157,35,372,111,424,210,184,414,97,246,221,467,
	422,113,382,300,23,70,342,360,289,183,386,441,177,292,389,345,12,337,49,353,19,384,164,
	159,333,434,310,137,8,415,138,395,52,482,460,325,92,439,409,139,261,101,453,396,61,284,
	373,188,514,63,469,219,263,368,194,287,96,168,264,365,230,27,237,150,528,379,203,491,39,
	80,276,29,403,236,327,78,525,445,151,318,483,44,195,507,294,72,1,329,216,128,426,303,
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

void Canvas::addStatic(uint8_t x, uint8_t y, uint8_t w, uint8_t h, float propToClear)
{
	dirty = true;
	uint16_t noiseThreshold = floor(propToClear * (float)(noiseSz * noiseSz));
	for (uint8_t i = x; i < x + w; ++i)
	{
		for (uint8_t j = y; j < y + h; ++j)
		{
			if (i >= 128 || j >= 64)
				continue;
			uint16_t nx = i % noiseSz;
			uint16_t ny = j % noiseSz;
			uint16_t nval = blueNoise[ny * noiseSz + nx];
			if (nval < noiseThreshold)
			{
				setPixel(i, j, false);
			}
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
	else if (font == font14)
		fd = &font14Data;
	else if (font == font30)
		fd = &font30Data;
	else if (font == font30N)
		fd = &font30NData;
	else if (font == wicons18)
		fd = &wicons18Data;
	else if (font == arrows14)
		fd = &arrows14Data;
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
