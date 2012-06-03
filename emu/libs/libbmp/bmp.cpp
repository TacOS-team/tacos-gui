#include "bmp.h"

#include <stdio.h>

typedef struct {
  unsigned short int type;
  unsigned int size;
  unsigned short int reserved1, reserved2;
  unsigned int offset;
} __attribute__ ((packed)) bmpfileheader_t;

typedef struct {
  unsigned int size;
  int width, height;
  unsigned short int planes;
  unsigned short int bits;
  unsigned int compression;
  unsigned int imagesize;
  int xresolution, yresolution;
  unsigned int ncolours;
  unsigned int importantcolours;
} __attribute__ ((packed)) bmpinfoheader_t;

BMPImageLoader::BMPImageLoader() {
  this->buff = NULL;
  this->width  = 0;
  this->height = 0;
}

char * BMPImageLoader::load(const std::string &fileName) {
  FILE *bmpfile = fopen(fileName.c_str(), "r");
  if(bmpfile == NULL) {
    printf("unable to open the bmp file '%s'\n", fileName.c_str());
    return NULL;
  }
  bmpfileheader_t fileHeader;
  bmpinfoheader_t infoHeader;

  if((int)fread(&fileHeader, sizeof(fileHeader), 1, bmpfile) == -1) {
    return NULL;
  }  
  if((int)fread(&infoHeader, sizeof(infoHeader), 1, bmpfile) == -1) {
    return NULL;
  }    

  this->width  = infoHeader.width;
  this->height = infoHeader.height;

  this->buff = new writeColor_t[this->width*this->height];

  if((int)fseek(bmpfile, fileHeader.offset, SEEK_SET) == -1) {
    return NULL;
  } 

  readColor_t  readPixel;
  writeColor_t writePixel;

  printf("width  : %d\n", this->width);
  printf("height : %d\n", this->height);

  for (int y = this->height - 1; y >= 0; y--) {
    for (int x = 0; x < this->width; x++) {
      printf("%d, %d : %d\n", x,y, y*this->width+x);
      fread(&readPixel, sizeof(readPixel), 1, bmpfile);
      writePixel.r = readPixel.r;
      writePixel.g = readPixel.g;
      writePixel.b = readPixel.b;
      /*writePixel.r = readPixel.r;
      writePixel.g = readPixel.g;
      writePixel.b = readPixel.b;*/
      this->buff[y*this->width+x] = writePixel;
    }

    int pos = ftell(bmpfile);
    if (pos % 4 != 0) {
      fseek(bmpfile, 4 - pos % 4, SEEK_CUR);
    }
  }

  return (char*)this->buff;
}

char * BMPImageLoader::getBuffer() {
  return (char*)this->buff;
}