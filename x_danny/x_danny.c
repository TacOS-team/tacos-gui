#include <stdlib.h>
#include <stdio.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <unistd.h>
#include "images.h"

/**
 * Danny program
 */
int main (int argc, char **argv){
  
  struct image_info * image = malloc(sizeof(struct image_info));
  struct pixel * im = lireCoulImage ("figures/kim.ppm",image);
  
  //infos de l'image chargée en mémoire
  printf("ma danny image fait %d par %d\n", image->width, image->height);

  //on se connecte au display
  Display * display = XOpenDisplay(NULL);

  if(display == NULL){
    fprintf(stderr,"Error connecting to the local display\n");
    return 1;
  }
  
  int display_width = DisplayWidth(display, 0);
  int display_height = DisplayHeight(display, 0);
  
  //retourner des informations sur le display
  printf("Width : %d \n", display_width);
  printf("Height : %d \n", display_height);
  printf("Display planes : %d \n", XDisplayPlanes(display, 0));
  printf("Display cells : %d \n", XDisplayCells(display, 0));
  printf("Display Width : %d \n", XDisplayWidthMM(display, 0));
  printf("Display Height : %d \n", XDisplayHeightMM(display, 0));
  
  //creation d'une fenêtre simple
  Window window = XCreateSimpleWindow(display, RootWindow(display,0), 1, 1, display_width, display_height,0, BlackPixel (display, 0), BlackPixel(display, 0));
  
  XMapWindow(display, window);
  
  XSelectInput(display, window, ExposureMask | KeyPressMask | ButtonPressMask);
  
  //dessine un danny
  GC green_gc;
  XColor green_col;
  Colormap colormap;
  char green[] = "#00FF00";
  XEvent report;

  colormap = DefaultColormap(display, 0);
  green_gc = XCreateGC(display, window, 0, 0);
  if(XParseColor(display, colormap, green, &green_col) == 0){
    fprintf(stderr,"Error parsing the color\n");
    return 1;
  }
  XAllocColor(display, colormap, &green_col);
  XSetForeground(display, green_gc, green_col.pixel);
  XDrawRectangle(display, window, green_gc, 1, 1, 497, 497);
  XDrawRectangle(display, window, green_gc, 50, 50, 398, 398);


  //on dessine une image

  /*
  for (y = 0 ; y < image_height ; y++) {
    for (x = 0 ; x < image_width ; x++) {
      
      if (nplanes > 8) {
	red >>= (8 - red_bits);
	green >>= (8 - green_bits);
	blue >>= (8 - blue_bits);
	
	pixel = ((red << red_shift) & red_mask) | ((green << green_shift) & green_mask) 
	  | ((blue << blue_shift) & blue_mask);
	
      }
      
      //8 planes
      else {
	XColor color;
	
	red <<= 8;
	green <<= 8;
	blue <<= 8;
	
	for (i = 0 ; i != last_color ; i++) {
	  if (color_cache[i].red == red && color_cache[i].green == green 
	      && color_cache[i].blue == blue)
	    break;
	}
	
	if (i == last_color) {
	  color.red = red;
	  color.green = green;
	  color.blue = blue;
	  
	  if (!XAllocColor (display, current_cmap, &color)) {
	    fprintf (stderr, "Can't allocate %d %d %d\n", red, green, blue);
	    exit (1);
	  }
	  else {
	    color_cache[i].red = red;
	    color_cache[i].green = green;
	    color_cache[i].blue = blue;
	    color_cache[i].pixel = pixel = color.pixel;
	  }
	  
	  last_color++;
	  if (last_color > 256)  {
	    fprintf (stderr, "Too many colors...\n");
	    exit (1);
	  }
	}
      }
    }
  }
*/	
	
  while (1)  {
    XNextEvent(display, &report);
    switch  (report.type) {
    case Expose:   
      fprintf(stdout, "I have been exposed.\n");
      XDrawRectangle(display, window, green_gc, 1, 1, 497, 497);
      XDrawRectangle(display, window, green_gc, 50, 50, 398, 398);
      XFlush(display);
      break;
    case KeyPress:
      //Close the program if q is pressed.
      if (XLookupKeysym(&report.xkey, 0) == XK_q) {
	exit(0);
      }
      break;
    }
  }

  //on ferme le display
  XCloseDisplay(display);
  return 0;
}
