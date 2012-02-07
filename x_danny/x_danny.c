#include <stdlib.h>
#include <stdio.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <unistd.h>

/**
 * Danny program
 */
int main (int argc, char **argv){
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
  /*if(colormap == NULL){
    fprintf(stderr,"Error creating the colormap\n");
    return 1;
    }*/
  green_gc = XCreateGC(display, window, 0, 0);
  if(XParseColor(display, colormap, green, &green_col) == 0){
    fprintf(stderr,"Error parsing the color\n");
    return 1;
  }
  XAllocColor(display, colormap, &green_col);
  XSetForeground(display, green_gc, green_col.pixel);
  XDrawRectangle(display, window, green_gc, 1, 1, 497, 497);
  XDrawRectangle(display, window, green_gc, 50, 50, 398, 398);

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
      /*Close the program if q is pressed.*/
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
