#ifndef _FONT_LOADER_H_
#define _FONT_LOADER_H_

#include <font.h>
#include <stdio.h>

class FontLoader {
public:
	static Font* load(char *fileName);

private:
	static void readProperties(int nb);
	static void readChars(int nb);

	static bool eq(const char *a, const char *b);

	static FILE *fd;
	static char kw[100];
	static char fontName[256];
	static Font::glyph_t fontGlyphs[256];
	static int pixelSize, fontAscent, fontDescent, fontHeight, defaultChar;
};

#endif