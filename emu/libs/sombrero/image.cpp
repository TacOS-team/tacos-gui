#include <image.h>
#include <stdio.h>
#include <stdlib.h>
#include <application.h>
#include <bmp.h>
#include <math.h>

namespace sombrero {


Image::Image(const string &filename) 
    :  Widget(), filename(filename) {
  this->init();
}

Image::~Image() { 
  pron::pronClearWindow(Application::getInstance()->d, this->pronWindow);
  pron::pronFreePixmap(Application::getInstance()->d, this->pixmap);
}

void Image::draw() {
  pron::pronCopyArea(Application::getInstance()->d, this->pixmap,
      this->pronWindow, Application::getInstance()->d->defaultGC, 0, 0,
      this->imageWidth, this->imageHeight, this->xOffset, this->yOffset);
}

void Image::init() {
  this->xOffset = 0;
  this->yOffset = 0;
  this->pixmap = 0;

  /* we will be using this uninitialized pointer later to store raw, uncompressd image */
  this->rawImage = NULL;

  if(filename[filename.length()-1] == 'p') {
    BMPImageLoader bmpLoader;
    this->rawImage = (unsigned char *)bmpLoader.load(filename);

    if(this->rawImage == NULL) {
        fprintf(stderr, "Error opening bmp file %s.\n", filename.c_str());
        exit(2);
    }

    this->imageWidth  = bmpLoader.width;
    this->imageHeight = bmpLoader.height;
    this->setWidth(bmpLoader.width);
    this->setHeight(bmpLoader.height);
    this->nbComponents = 3;
  }
  else {
    /* these are standard libjpeg structures for reading (decompression) */
    struct jpeg_decompress_struct cinfo;
    struct jpeg_error_mgr jerr;
    /* libjpeg data structure for storing one row, that is, scanline of an image */
    JSAMPROW row_pointer[1];

    FILE *infile = fopen(filename.c_str(), "rb");
    unsigned long location = 0;

    if (infile == NULL) {
      fprintf(stderr, "Error opening jpeg file %s.\n", filename.c_str());
      exit(2);
    }

    /* here we set up the standard libjpeg error handler */
    cinfo.err = jpeg_std_error(&jerr);
    /* setup decompression process and source, then read JPEG header */
    jpeg_create_decompress(&cinfo);
    /* this makes the library read from infile */
    jpeg_stdio_src(&cinfo, infile);
    /* reading the image header which contains image information */
    jpeg_read_header(&cinfo, TRUE);

    /* Uncomment the following to output image information, if needed. */
    //*--
    printf("JPEG File Information: \n");
    printf("Image width and height: %d pixels and %d pixels.\n", cinfo.image_width, cinfo.image_height);
    printf("Color components per pixel: %d.\n", cinfo.num_components);
    printf("Color space: %d.\n", cinfo.jpeg_color_space);
    //--*/
    
    this->imageWidth = cinfo.image_width;
    this->imageHeight = cinfo.image_height;
    this->nbComponents = cinfo.num_components;
    this->setWidth(this->imageWidth);
    this->setHeight(this->imageHeight);

    /* Start decompression jpeg here */
    jpeg_start_decompress(&cinfo);

    /* allocate memory to hold the uncompressed image */
    this->rawImage = (unsigned char*) malloc(cinfo.output_width * cinfo.output_height * cinfo.num_components);
    /* now actually read the jpeg into the raw buffer */
    row_pointer[0] = (unsigned char *) malloc(cinfo.output_width * cinfo.num_components);
    /* read one scan line at a time */
    while (cinfo.output_scanline < cinfo.image_height) {
      jpeg_read_scanlines(&cinfo, row_pointer, 1);
      for (unsigned int i = 0; i < cinfo.image_width * cinfo.num_components; i += cinfo.num_components) {
        for (int j = cinfo.num_components - 1; j >= 0; j--) {
          this->rawImage[location++] = row_pointer[0][i + j];
        }
      }
    }

    /* wrap up decompression, destroy objects, free pointers and close open files */
    jpeg_finish_decompress(&cinfo);
    jpeg_destroy_decompress(&cinfo);
    free(row_pointer[0]);
    fclose(infile);
  }

  this->sendPixmap(true);
}

void Image::sendPixmap(bool createNew) {
  if (createNew) {
    // if the pixmap has already been created
    if (this->pixmap != 0) {
      pron::pronClearWindow(Application::getInstance()->d, this->pronWindow);
      pron::pronFreePixmap(Application::getInstance()->d, this->pixmap);
    }
  
    /* Creates the image pixmap */
    this->pixmap = pron::pronCreatePixmap(Application::getInstance()->d,
        this->imageWidth, this->imageHeight, 24);
    if (this->pixmap == (unsigned int)-1) {
      fprintf(stderr, "Error while creating the pixmap\n");
      exit(2);
    }
  }

  /* Create PronImage */
  pron::PronImage image(this->imageWidth, this->imageHeight, pron::ZPixmap, (char *)this->rawImage, 24, this->nbComponents, false);

  /* Puts the image into the pixmap */
  pron::pronPutImage(Application::getInstance()->d, pixmap,
    Application::getInstance()->d->defaultGC, &image, 0, 0,
    this->imageWidth, this->imageHeight, 0, 0);
}

void Image::applyNegativeFilter() {
  unsigned long location = 0;
  for (unsigned int h = 0 ; h < this->imageHeight; h++) {
    for (unsigned int i = 0; i < this->imageWidth * this->nbComponents; i += this->nbComponents) {
      for (int j = this->nbComponents - 1; j >= 0; j--) {
        this->rawImage[location] = 255 - this->rawImage[location];
        location++;
      }
    }
  }
  this->sendPixmap(false);
}

unsigned int Image::getLocation(unsigned int i, unsigned int j, unsigned int c, unsigned int currentWidth) {
  return this->nbComponents * (i + j * currentWidth) + c;
}

void Image::rotate(bool clockwise) {
  unsigned char * newRawImage = (unsigned char *)malloc(this->imageHeight * this->imageWidth * this->nbComponents * sizeof(unsigned char));
  
  unsigned int newImageWidth = this->imageHeight;
  unsigned int newImageHeight = this->imageWidth;

  for (unsigned int i = 0; i < newImageWidth; i++) {
    for (unsigned int j = 0; j < newImageHeight; j++) {
      for (unsigned int c = 0; c < this->nbComponents; c++) {
        if (clockwise) {
          newRawImage[this->getLocation(i,j,c, newImageWidth)] = this->rawImage[this->getLocation(j, this->imageHeight - i -1, c, this->imageWidth)];
        } else {
          newRawImage[this->getLocation(i,j,c, newImageWidth)] = this->rawImage[this->getLocation(this->imageWidth - j -1,i, c, this->imageWidth)];
        }
      }
    }
  }
  
  memcpy(this->rawImage,newRawImage,this->imageHeight * this->imageWidth * this->nbComponents * sizeof(unsigned char));

  this->imageWidth = newImageWidth;
  this->imageHeight = newImageHeight;
  this->setWidth (this->imageWidth);
  this->setHeight(this->imageHeight);

  this->sendPixmap(true);
}


unsigned char Image::calculateFonkNewComp (unsigned int i, unsigned int j, unsigned int c, unsigned char * raw) {
  int newComp = 0;
  int blop = 1;

  newComp += (char)raw[this->getLocation(i,j,c,this->imageWidth)];
  if (i + 1 < this->imageWidth) {
    newComp += (char)raw[this->getLocation(i+1,j,c,this->imageWidth)];
    blop++;
    if (j > 0) {
      newComp += (char)raw[this->getLocation(i+1,j-1,c,this->imageWidth)];
      blop++;
    }
    if (j + 1 < this->imageHeight) {
      newComp += (char)raw[this->getLocation(i+1,j+1,c,this->imageWidth)];
      blop++;
    }
  }
  if (i > 0) {
    newComp += (char)raw[this->getLocation(i-1,j,c,this->imageWidth)];
    blop++;
    if (j > 0) {
      newComp += (char)raw[this->getLocation(i-1,j-1,c,this->imageWidth)];
      blop++;
    }
    if (j + 1 < this->imageHeight) {
      newComp += (char)raw[this->getLocation(i-1,j+1,c,this->imageWidth)];
      blop++;
    }
  }
  if (j > 0) {
    newComp += (char)raw[this->getLocation(i,j-1,c,this->imageWidth)];
    blop++;
  } 
  if (j + 1 < this->imageHeight) {
    newComp += (char)raw[this->getLocation(i,j+1,c,this->imageWidth)];
    blop++;
  }

  newComp /= blop;

  return (char)newComp;
}

/*
 * Using a convolution matrix would be better but we need signed operations to apply the weird aweseome effect
 */
void Image::applyPowerfullnessOfTheFonkFilter() {
  unsigned char * newRawImage = (unsigned char *)malloc(this->imageHeight * this->imageWidth * this->nbComponents * sizeof(unsigned char));
  unsigned char * rawImages [2]= {newRawImage,this->rawImage}; // used to avoid memcpy at each loop
  int currentRawImage = 0;
  
  unsigned int nbPass = 4;
  for (unsigned int pass = 0; pass < nbPass; pass++) {
    for (unsigned int i = 0; i < this->imageWidth; i++) {
      for (unsigned int j = 0; j < this->imageHeight; j++) {
        for (unsigned int c = 0; c < this->nbComponents; c++) {
          rawImages[currentRawImage][this->getLocation(i,j,c, this->imageWidth)] = this->calculateFonkNewComp(i,j,c,rawImages[(currentRawImage + 1) % 2]);
        }
      }
    }
    currentRawImage = (currentRawImage + 1) % 2;
  }
  // We memcpy only if we have to
  if (currentRawImage == 0) {
    memcpy(rawImages[1],rawImages[0],this->imageHeight * this->imageWidth * this->nbComponents * sizeof(unsigned char));
  }
  
  memcpy(this->rawImage, newRawImage,this->imageHeight * this->imageWidth * this->nbComponents * sizeof(unsigned char));
  this->sendPixmap(false);
}

unsigned char Image::calculateNewComp(unsigned int i, unsigned int j, unsigned int c, unsigned char * raw, unsigned int rawWidth, unsigned int rawHeight, int ** convMat, unsigned int convMatSize) {
  int newComp = 0;
  int middle = (convMatSize / 2);
  int realI = 0; // The column number of the current adjacent pixel
  int realJ = 0; // The column number of the current adjacent pixel
  int sum = 0;
  if ((convMatSize % 2) != 1) {
    printf("size of the convolution matrix must be odd\n");
  } else {
    for (unsigned int convI = 0; convI < convMatSize; convI++) {
      for (unsigned int convJ = 0; convJ < convMatSize; convJ++) {
        realI = i - (middle - convI);
        realJ = j - (middle - convJ);
        if (realI >= 0 && realI < (int)rawWidth && realJ >= 0 && realJ < (int)rawHeight) {
          newComp += raw[this->getLocation(realI,realJ,c, rawWidth)] * convMat[convI][convJ];
          sum += abs(convMat[convI][convJ]);
        }
      }
    }
  }
  
  newComp /= sum;
  return (unsigned char)newComp;
}

void Image::applyGaussianBlurFilter() {
  unsigned char * newRawImage = (unsigned char *)malloc(this->imageHeight * this->imageWidth * this->nbComponents * sizeof(unsigned char));
  int ** convMat;
  unsigned int convMatSize = 3;
  convMat = (int **)malloc(convMatSize * sizeof(int *));
  for (unsigned int i = 0; i < convMatSize; i++) {
    *(convMat + i) = (int *)malloc(convMatSize * sizeof(int));
  }

  convMat[0][0] = 1;
  convMat[1][0] = 2;
  convMat[2][0] = 1;
  convMat[0][1] = 2;
  convMat[1][1] = 4;
  convMat[2][1] = 2;
  convMat[0][2] = 1;
  convMat[1][2] = 2;
  convMat[2][2] = 1;

  for (unsigned int i = 0; i < this->imageWidth; i++) {
    for (unsigned int j = 0; j < this->imageHeight; j++) {
      for (unsigned int c = 0; c < this->nbComponents; c++) {
        newRawImage[this->getLocation(i,j,c, this->imageWidth)] = calculateNewComp(i, j, c, this->rawImage, this->imageWidth, this->imageHeight, convMat, convMatSize);;
      }
    }
  }
  
  memcpy(this->rawImage, newRawImage,this->imageHeight * this->imageWidth * this->nbComponents * sizeof(unsigned char));
  this->sendPixmap(false);
}

unsigned int Image::getImageWidth(){
  return this->imageWidth;
}
unsigned int Image::getImageHeight(){
  return this->imageHeight;
}

void Image::setXOffset(int newXOffset) {
  this->xOffset = newXOffset;
}

void Image::setYOffset(int newYOffset) {
  this->yOffset = newYOffset;
}

pron::Pixmap Image::getPixMap() {
  return this->pixmap;
}

} // namespace sombrero
