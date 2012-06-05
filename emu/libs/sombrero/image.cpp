#include <image.h>
#include <stdio.h>
#include <stdlib.h>
#include <application.h>
#include <bmp.h>

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

pron::Pixmap Image::getPixMap() {
  return this->pixmap;
}

void Image::init() {
  this->xOffset = 0;
  this->yOffset = 0;

  /* we will be using this uninitialized pointer later to store raw, uncompressd image */
  this->rawImage = NULL;

  if(filename[filename.length()-1] == 'p') {
    BMPImageLoader bmpLoader;
    this->rawImage = bmpLoader.load(filename);

    if(this->rawImage == NULL) {
        fprintf(stderr, "Error opening bmp file %s.\n", filename.c_str());
        exit(2);
    }

    this->imageWidth  = bmpLoader.width;
    this->imageHeight = bmpLoader.height;
    this->setWidth(bmpLoader.width);
    this->setHeight(bmpLoader.height);
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
    this->rawImage = (char*) malloc(cinfo.output_width * cinfo.output_height * cinfo.num_components);
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
  
  /* Create PronImage */
  pron::PronImage image(this->imageWidth, this->imageHeight, pron::ZPixmap, this->rawImage, 24, 3, false);

  /* Creates the image pixmap */
  pixmap = pron::pronCreatePixmap(Application::getInstance()->d,
    this->imageWidth, this->imageHeight, 24);
  if (pixmap == (unsigned int)-1) {
    fprintf(stderr, "Error while creating the pixmap\n");
    exit(2);
  }

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
  
  /* Create PronImage */
  pron::PronImage image(this->imageWidth, this->imageHeight, pron::ZPixmap, this->rawImage, 24, 3, false);
  
  /* Puts the image into the pixmap */
  pron::pronPutImage(Application::getInstance()->d, pixmap,
    Application::getInstance()->d->defaultGC, &image, 0, 0,
    this->imageWidth, this->imageHeight, 0, 0);
}

unsigned int Image::getLocation(unsigned int i, unsigned int j, unsigned int c, unsigned int currentWidth) {
  return this->nbComponents * (i + j * currentWidth) + c;
}


void Image::rotateClockwise() {
  char * newRawImage = (char *)malloc(this->imageHeight * this->imageWidth * this->nbComponents * sizeof(char));
  
  unsigned int newImageWidth = this->imageHeight;
  unsigned int newImageHeight = this->imageWidth;

  for (unsigned int i = 0; i < newImageWidth; i++) {
    for (unsigned int j = 0; j < newImageHeight; j++) {
      for (unsigned int c = 0; c < this->nbComponents; c++) {
        newRawImage[this->getLocation(i,j,c, newImageWidth)] = this->rawImage[this->getLocation(j, this->imageHeight - i -1, c, this->imageWidth)];
        //newRawImage[3 * (i + j * this->imageHeight) + c] = this->rawImage[3 * (j + (this->imageHeight - i - 1) * this->imageWidth) + c];
        //newRawImage[this->accesMat(i,j,c)] = 255 - this->rawImage[this->accesMat(i, j, c)];
        //newRawImage[this->accesMat(i,j,c)] = this->rawImage[this->accesMat(this->imageWidth - j,i - 1, c)];
      }
    }
  }

  memcpy(this->rawImage,newRawImage,this->imageHeight * this->imageWidth * this->nbComponents * sizeof(char));

  pron::pronClearWindow(Application::getInstance()->d, this->pronWindow);
  pron::pronFreePixmap(Application::getInstance()->d, this->pixmap);

  this->imageWidth = newImageWidth;
  this->imageHeight = newImageHeight;
  this->setWidth (this->imageWidth);
  this->setHeight(this->imageHeight);

  /* Create PronImage */
  pron::PronImage image(this->imageWidth, this->imageHeight, pron::ZPixmap, this->rawImage, 24, 3, false);

  /* Creates the image pixmap */
  this->pixmap = pron::pronCreatePixmap(Application::getInstance()->d,
    this->imageWidth, this->imageHeight, 24);
  if (this->pixmap == (unsigned int)-1) {
    fprintf(stderr, "Error while creating the pixmap\n");
    exit(2);
  }

  /* Puts the image into the pixmap */
  pron::pronPutImage(Application::getInstance()->d, this->pixmap,
    Application::getInstance()->d->defaultGC, &image, 0, 0,
    this->imageWidth, this->imageHeight, 0, 0);
}




void Image::reverseImage() {
  char * newRawImage = (char *)malloc(this->imageHeight * this->imageWidth * this->nbComponents * sizeof(char));

  unsigned long location = 0;
  for (unsigned int h = 0 ; h < this->imageHeight; h++) {
    for (unsigned int i = 0; i < this->imageWidth * this->nbComponents; i += this->nbComponents) {
      for (int j = this->nbComponents - 1; j >= 0; j--) {
        newRawImage[location] = this->rawImage[(this->imageWidth * this->imageHeight * this->nbComponents) - location - (this->nbComponents - j - 1)];
        location++;
      }
    }
  }
  memcpy(this->rawImage,newRawImage,this->imageHeight * this->imageWidth * this->nbComponents * sizeof(char));
  
  /* Create PronImage */
  pron::PronImage image(this->imageWidth, this->imageHeight, pron::ZPixmap, newRawImage, 24, 3, false);
  
  /* Puts the image into the pixmap */
  pron::pronPutImage(Application::getInstance()->d, pixmap,
    Application::getInstance()->d->defaultGC, &image, 0, 0,
    this->imageWidth, this->imageHeight, 0, 0);
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

} // namespace sombrero
