#include <stdio.h>
#include <SDL/SDL.h>
#include "jpeglib.h"
#include "setjmp.h"

struct my_error_mgr {
  struct jpeg_error_mgr pub;	/* "public" fields */
  jmp_buf setjmp_buffer;	/* for return to caller */
};

typedef struct my_error_mgr * my_error_ptr;

METHODDEF(void) my_error_exit (j_common_ptr cinfo) {
  /* cinfo->err really points to a my_error_mgr struct, so coerce pointer */
  my_error_ptr myerr = (my_error_ptr) cinfo->err;

  /* Always display the message. */
  /* We could postpone this until after returning, if we chose. */
  (*cinfo->err->output_message) (cinfo);

  /* Return control to the setjmp point */
  longjmp(myerr->setjmp_buffer, 1);
}


void pix (SDL_Surface * screen, int x, int y, int r, int g, int b) {
  SDL_Surface *pix = SDL_CreateRGBSurface(SDL_HWSURFACE, 1, 1, 32, 0, 0, 0, 0);
  SDL_Rect pos;
  pos.x = x;
  pos.y = y;
  SDL_FillRect(pix, NULL, SDL_MapRGB( screen->format, r, g, b));
  SDL_BlitSurface(pix, NULL, screen, &pos);
}

int main() {
  SDL_Surface *screen;
  SDL_Event event;
  int keyPressed = 0;

  char * filename = "tacos.jpg";
  FILE * infile;		// .jpg source file
  struct my_error_mgr jerr; // error handler
  struct jpeg_decompress_struct cinfo; // information about the jpg file(height, width, etc)
  JSAMPARRAY buffer; // the buffer that contains the read lines, one by one
  int sizeOfBuffer; 

  unsigned char r = 0;
  unsigned char g = 0;
  unsigned char b = 0;
  int i = 0;

  if ((infile = fopen(filename, "rb")) == NULL) {
       fprintf(stderr, "can't open %s\n", filename);
    return 0;
  }
  
  cinfo.err = jpeg_std_error(&jerr.pub);
  jerr.pub.error_exit = my_error_exit;
  if (setjmp(jerr.setjmp_buffer)) {
    jpeg_destroy_decompress(&cinfo);
    fclose(infile);
    return 0;
  }

  jpeg_create_decompress(&cinfo);

  jpeg_stdio_src(&cinfo, infile);

  (void) jpeg_read_header(&cinfo, TRUE);


  (void) jpeg_start_decompress(&cinfo);
  // Here the cinfo structure is complete and we can use it

  if (SDL_Init(SDL_INIT_VIDEO) < 0 ) { 
    return EXIT_FAILURE;
  }
  if (!(screen = SDL_SetVideoMode(cinfo.output_width, cinfo.output_height, 32, SDL_HWSURFACE))) {
    SDL_Quit();
    return 1;
  }
  printf("width : %d\nheight : %d\n",cinfo.output_width, cinfo.output_height);


  sizeOfBuffer = cinfo.output_width * cinfo.output_components; 
  buffer = (*cinfo.mem->alloc_sarray) ((j_common_ptr) &cinfo, JPOOL_IMAGE, sizeOfBuffer, 1); // Allocating the buffer

  while (cinfo.output_scanline < cinfo.output_height) { // We start scanning lines one by one
    jpeg_read_scanlines(&cinfo, buffer, 1); 
    // The decompressed line is stored in buffer[0] and it looks like this
    //     R G B         R G B            R G B    ...
    // first pixel   second pixel      third pixel ...
    
    // We scan each pixel and we print it on the screen
    for (i = 0; i < sizeOfBuffer; i+=3) {
      r = (int) buffer[0][i+0];
      g = (int) buffer[0][i+1];
      b = (int) buffer[0][i+2];
      pix(screen,i/3, cinfo.output_scanline,r,g,b); 
    }
  }

  jpeg_finish_decompress(&cinfo);
  jpeg_destroy_decompress(&cinfo);
  fclose(infile);


  while(!keyPressed) {
    while(SDL_PollEvent(&event)) {      
      switch (event.type) {
        case SDL_QUIT:
          keyPressed = 1;
          break;
        case SDL_KEYDOWN:
          keyPressed = 1;
          break;
        default:
          break;
      }
    }
  }

  SDL_Quit();

  return 0;
}
