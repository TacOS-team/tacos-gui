#ifndef _FONT_H_
#define _FONT_H_

class Font {
public:
	static const int MAX_HEIGHT = 16;
	
	struct glyph_t {
		int width;
		unsigned short bitmap[MAX_HEIGHT];
	};

	Font(char *name, int height, glyph_t glyphs[256]);
	void drawChar(int x, int y, unsigned char car);
	void drawText(int x, int y, const char *text, int length);
	void printInfo();

private:
	char *name;
	int height;
	glyph_t glyphs[256];
};

#endif