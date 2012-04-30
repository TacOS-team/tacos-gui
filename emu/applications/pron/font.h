#ifndef _FONT_H_
#define _FONT_H_

#include <drawable.h>

class Font {
public:
	static const int MAX_HEIGHT = 16;
	
	struct glyph_t {
		int width;
		unsigned short bitmap[MAX_HEIGHT];
	};

	Font(char *name, int height, glyph_t glyphs[256]);
  void textSize(const char *text, int length, int *width, int *height);
	void drawChar(Drawable *d, int x, int y, unsigned char car);
	void drawText(Drawable *d, int x, int y, const char *text, int length);
	void printInfo();

private:
	char *name;
	int height;
	glyph_t glyphs[256];
};

#endif
