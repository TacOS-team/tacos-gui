#include <font.h>
#include <font_loader.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

FILE* FontLoader::fd;
char FontLoader::kw[100];
char FontLoader::fontName[256];
Font::glyph_t FontLoader::fontGlyphs[256];
int FontLoader::pixelSize, FontLoader::fontAscent, FontLoader::fontDescent, FontLoader::fontHeight, FontLoader::defaultChar;

bool FontLoader::eq(const char *a, const char *b) {
	return (strcmp(a, b) == 0);
}

void FontLoader::readProperties(int nb) {
	for (int i = 0; i < nb; i++) {
		fscanf(fd, "%s", kw);
		//fprintf(stderr, "Read %s\n", kw);
		if (eq(kw, "PIXEL_SIZE")) {
			fscanf(fd, "%d", &pixelSize);
		} else if (eq(kw, "FONT_ASCENT")) {
			fscanf(fd, "%d", &fontAscent);
		} else if (eq(kw, "FONT_DESCENT")) {
			fscanf(fd, "%d", &fontDescent);
		} else if (eq(kw, "DEFAULT_CHAR")) {
			fscanf(fd, "%d", &defaultChar);
		} else {
			//fprintf(stderr, "Ignored keyword: %s\n", kw);
			while (fgetc(fd) != '\n');
		}
	}

	fontHeight = fontAscent + fontDescent;

	fscanf(fd, "%s", kw);
	if (!eq(kw, "ENDPROPERTIES")) {
		fprintf(stderr, "Fatal: expected ENDPROPERTIES, got %s.", kw);
		exit(2);
	}
}

void FontLoader::readChars(int nb) {
	for (int i = 0; i < nb; i++) {
		char glyphName[100];
		fscanf(fd, "%s %s", kw, glyphName);
		if (!eq(kw, "STARTCHAR")) {
			fprintf(stderr, "Fatal: no STARTCHAR found.\n");
			exit(2);
		}
		bool end = false;
		int encoding = -1, width = -1, bbw = -1, bbh = -1, bbx = 0, bby = 0, line, unused;
		while (!end) {
			fscanf(fd, "%s", kw);
			if (eq(kw, "ENCODING")) {
				fscanf(fd, "%d", &encoding);
			} else if (eq(kw, "DWIDTH")) {
				fscanf(fd, "%d %d", &width, &unused);
			} else if (eq(kw, "BBX")) {
				fscanf(fd, "%d %d %d %d", &bbw, &bbh, &bbx, &bby);
			} else if (eq(kw, "BITMAP")) {
				if (encoding == -1) {
					//fprintf(stderr, "No encoding set - skipping character.\n");
					continue;
				} else if (bbw == -1 || bbh == -1) {
					//fprintf(stderr, "No bounding box set - skipping character.\n");
					continue;
				} else if (width == -1) {
					//fprintf(stderr, "No width set - skipping character.\n");
					continue;
				}

				width -= bbx;
				fontGlyphs[encoding].width = width;

				if (bbx < 0) {
					bbx = 0;
				}

				for (int i = 0; i < bbh; i++) {
					fscanf(fd, "%x", &line);
					int nbDigits = 0;
					while (line >> (4 * (++nbDigits)));
					fontGlyphs[encoding].bitmap[fontHeight - fontDescent - bbh - bby + i] = line << (16 - 4 * nbDigits - bbx);
				}
			} else if (eq(kw, "ENDCHAR")) {
				end = true;
			} else {
				//fprintf(stderr, "Ignored keyword: %s\n", kw);
				while (fgetc(fd) != '\n');
			}
		}
	}
}

Font* FontLoader::load(char *fileName) {
	fd = fopen(fileName, "r");
	if (fd == NULL) {
		perror("Unable to open file");
		exit(1);
	}

	char version[10];
	if (fscanf(fd, "STARTFONT %s", version) <= 0) {
		fprintf(stderr, "Fatal: no STARTFONT found.");
		exit(2);
	} else {
		printf("Reading font v%s\n", version);
		bool end = false;
		while (!end) {
			fscanf(fd, "%s", kw);
			if (eq(kw, "FONT")) {
				fscanf(fd, "%s", fontName);
			} else if (eq(kw, "STARTPROPERTIES")) {
				int nbProperties;
				fscanf(fd, "%d", &nbProperties);
				readProperties(nbProperties);
			} else if (eq(kw, "CHARS")) {
				int nbChars;
				fscanf(fd, "%d", &nbChars);
				readChars(nbChars);
			} else if (eq(kw, "ENDFONT")) {
				end = true;
			} else {
				//fprintf(stderr, "Ignored keyword: %s\n", kw);
				while (fgetc(fd) != '\n');
			}
		}

		return new Font(fontName, fontHeight, fontGlyphs);
	}
}
