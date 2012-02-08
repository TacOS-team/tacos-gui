#include <stdlib.h>
#include <stdio.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include "images.h"

/**
 * Danny program
 */
int main (int argc, char **argv){
  
  int fullscreen = 0;
  int lol_mode = 0;
  int c;

  if(argc > 1){
    fullscreen = 1;
  }

  while ((c = getopt (argc, argv, "fl")) != -1){
    switch (c){
    case 'f':
      fullscreen = 1;
      break;
    case 'l':
      lol_mode = 1;
      break;
    default:
      abort ();
    }
  }

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
  int window_width;
  int window_height;
  if(fullscreen){
    window_width = display_width;
    window_height = display_height;
  } else {
    window_width = image->width;
    window_height = image->height;
  }

  Window window = XCreateSimpleWindow(display, RootWindow(display,0), 0, 0, 
				      window_width, window_height, 0, 
				      BlackPixel (display, 0), WhitePixel(display, 0));
  XSetWindowBorderWidth(display, window, 0);
  XMapWindow(display, window);  
  XSelectInput(display, window, ExposureMask | KeyPressMask | ButtonPressMask | EnterNotify);
  XSetWindowAttributes xswa;
  xswa.override_redirect = True;
  XChangeWindowAttributes(display, window, CWOverrideRedirect, &xswa);
  //on demande le focus
  XRaiseWindow(display, window);
  
  //dessine un danny
  XEvent report;  
  //creation de l'image
  int screen = DefaultScreen(display);
  Visual * visual = DefaultVisual(display, screen);
  int depth = DefaultDepth(display, screen);
   
  printf("depth %d\n", depth);
  XImage * xi = XCreateImage(display, visual, depth, ZPixmap, 0,
			     (char *)im, image->width, image->height, 32, 0);

  int x, y;
  time_t t;
  int has_finished = 0;
  while (!has_finished)  {
    XNextEvent(display, &report);
    switch  (report.type) {
    case Expose:   
      fprintf(stdout, "I have been exposed.\n");
      XPutImage(display, window, DefaultGC(display,0), xi, 0, 0, 0, 0, image->width, image->height);
      XFlush(display);
      break;
    case KeyPress:
      /*if(lol_mode){
	time(&t);
	x = (int)(rand() / (double)RAND_MAX * ( window_width - image->width));
	y = (int)(rand() / (double)RAND_MAX * ( window_height - image->height));
	XPutImage(display, window, DefaultGC(display,0), xi, 0, 0, x, y, image->width, image->height);
	XFlush(display);
	printf("x %d y %d \n",x,y);
	}*/
      //Close the program if q is pressed.
      if (XLookupKeysym(&report.xkey, 0) == XK_q) {
	has_finished = 1;
      }
      break;
    case EnterNotify:
      if(lol_mode){
	time(&t);
	x = (int)(rand() / (double)RAND_MAX * ( window_width - image->width));
	y = (int)(rand() / (double)RAND_MAX * ( window_height - image->height));
	XPutImage(display, window, DefaultGC(display,0), xi, 0, 0, x, y, image->width, image->height);
	XFlush(display);
	printf("x %d y %d \n",x,y);
      }
      break;
    }
  }

  //on ferme le display
  XCloseDisplay(display);
  return 0;
}
