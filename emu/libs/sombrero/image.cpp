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
  if (this->isReversed) {
    pron::pronNegArea(Application::getInstance()->d, this->pixmap,
      this->pronWindow, Application::getInstance()->d->defaultGC, 0, 0,
      this->imageWidth, this->imageHeight, this->xOffset, this->yOffset);
  } else {
    pron::pronCopyArea(Application::getInstance()->d, this->pixmap,
      this->pronWindow, Application::getInstance()->d->defaultGC, 0, 0,
      this->imageWidth, this->imageHeight, this->xOffset, this->yOffset);
  }
}

pron::Pixmap Image::getPixMap() {
  return this->pixmap;
}

void Image::init() {
  this->isReversed = false;
  this->xOffset = 0;
  this->yOffset = 0;

  /* we will be using this uninitialized pointer later to store raw, uncompressd image */
  char *raw_image = NULL;

  if(filename[filename.length()-1] == 'p') {
    BMPImageLoader bmpLoader;
    raw_image = bmpLoader.load(filename);

    if(raw_image == NULL) {
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
    this->setWidth(this->imageWidth);
    this->setHeight(this->imageHeight);

    /* Start decompression jpeg here */
    jpeg_start_decompress(&cinfo);

    /* allocate memory to hold the uncompressed image */
    raw_image = (char*) malloc(cinfo.output_width * cinfo.output_height * cinfo.num_components);
    /* now actually read the jpeg into the raw buffer */
    row_pointer[0] = (unsigned char *) malloc(cinfo.output_width * cinfo.num_components);
    /* read one scan line at a time */
    while (cinfo.output_scanline < cinfo.image_height) {
      jpeg_read_scanlines(&cinfo, row_pointer, 1);
      for (unsigned int i = 0; i < cinfo.image_width * cinfo.num_components; i += cinfo.num_components) {
        //for (unsigned int j = 0; j < cinfo.num_components; j++) {
        for (int j = cinfo.num_components - 1; j >= 0; j--) {
          raw_image[location++] = row_pointer[0][i + j];
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
  pron::PronImage image(this->imageWidth, this->imageHeight, pron::ZPixmap, raw_image, 24, 3, false);

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

unsigned int Image::getImageWidth(){
  return this->imageWidth;
}
unsigned int Image::getImageHeight(){
  return this->imageHeight;
}

void Image::reverseColors() {
  this->isReversed = ! this->isReversed;
}

void Image::setXOffset(int newXOffset) {
  this->xOffset = newXOffset;
}

void Image::setYOffset(int newYOffset) {
  this->yOffset = newYOffset;
}

} // namespace sombrero
