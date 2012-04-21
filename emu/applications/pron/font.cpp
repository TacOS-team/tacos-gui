#include <font.h>
#include <screen.h>
#include <stdio.h>
#include <string.h>

Font::Font(char *name, int height, glyph_t glyphs[256]) {
  this->name = strdup(name);
  this->height = height;
  memcpy(this->glyphs, glyphs, sizeof(glyph_t[256]));

  //this->printInfo();
}

void Font::drawChar(int x, int y, unsigned char car) {
  for (int l = 0; l < this->height; l++) {
    for (int c = 0; c < this->glyphs[car].width; c++) {
      int ci = 16 - 1 - c;
      if (this->glyphs[car].bitmap[l] & (1 << ci)) {
        Screen::getInstance()->drawPoint(x + c, y + l);
      }
    }
  }
}

void Font::drawText(int x, int y, const char *text, int length) {
  y -= (this->height - 1);
  for (int i = 0; i < length; i++) {
    unsigned char car = text[i];
    this->drawChar(x, y, car);
    x += this->glyphs[car].width;
  }
}

void Font::printInfo() {
  printf("Font name: %s, height: %d\n", this->name, this->height);
  printf("Glyphs:\n");
  for (int i = 0; i < 255; i++) {
    printf("Char %d:\n", i);
    for (int l = 0; l < this->height; l++) {
      for (int c = 15; c >= 0; c--) {
        if (this->glyphs[i].bitmap[l] & (1 << c)) {
          printf("*");
        } else {
          printf(" ");
        }
      }
      printf("\n");
    }
    printf("\n");
  }
}