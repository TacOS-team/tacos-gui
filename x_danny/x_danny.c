#include <stdlib.h>
#include <stdio.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include <assert.h>
#include <math.h>
#include "images.h"

#define X_DANNY_EYE_LEFT_X 34
#define X_DANNY_EYE_LEFT_Y 64
#define X_DANNY_EYE_LEFT_RADIUS 20
#define X_DANNY_EYE_LEFT_PUPIL_RADIUS 10

#define X_DANNY_EYE_RIGHT_X 73
#define X_DANNY_EYE_RIGHT_Y 70
#define X_DANNY_EYE_RIGHT_RADIUS 20
#define X_DANNY_EYE_RIGHT_PUPIL_RADIUS 10

/**
 * @author John Carmack
 */
float Q_rsqrt( float number ){
    long i;
    float x2, y;
    const float threehalfs = 1.5F;
 
    x2 = number * 0.5F;
    y  = number;
    i  = * ( long * ) &y;  // evil floating point bit level hacking
    i  = 0x5f3759df - ( i >> 1 ); // what the fuck?
    y  = * ( float * ) &i;
    y  = y * ( threehalfs - ( x2 * y * y ) ); // 1st iteration
    // y  = y * ( threehalfs - ( x2 * y * y ) ); // 2nd iteration, this can be removed
 
    #ifndef Q3_VM
    #ifdef __linux__
      assert( !isnan(y) ); // bk010122 - FPE?
    #endif
    #endif
    return y;
}

float pow2( float number ){
  return number * number;
}

/**
 * Danny program
 */
int main (int argc, char **argv){
  
  int fullscreen = 0;
  int lol_mode = 0;
  int move_mode = 0;
  int c;

  int eye_left_x = X_DANNY_EYE_LEFT_X;
  int eye_left_y = X_DANNY_EYE_LEFT_Y;
  int eye_left_radius = X_DANNY_EYE_LEFT_RADIUS;
  int eye_left_pupil_radius = X_DANNY_EYE_LEFT_PUPIL_RADIUS;

  int eye_right_x = X_DANNY_EYE_RIGHT_X;
  int eye_right_y = X_DANNY_EYE_RIGHT_Y;
  int eye_right_radius = X_DANNY_EYE_RIGHT_RADIUS;
  int eye_right_pupil_radius = X_DANNY_EYE_RIGHT_PUPIL_RADIUS;

  while ((c = getopt (argc, argv, "flm")) != -1){
    switch (c){
    case 'f':
      fullscreen = 1;
      break;
    case 'l':
      lol_mode = 1;
      break;
    case 'm':
      move_mode = 1;
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

  printf("hauteur %d , largeur %d\n", window_width, window_height);

  Window window = XCreateSimpleWindow(display, RootWindow(display,0), 0, 0, 
				      window_width, window_height, 0, 
				      BlackPixel (display, 0), WhitePixel(display, 0));
  XSetWindowBorderWidth(display, window, 0);
  XMapWindow(display, window);  
  XSelectInput(display, window, ExposureMask | KeyPressMask | ButtonPressMask | PointerMotionMask);
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

  //couleurs
  Colormap color_map = XCreateColormap(display, window, visual, AllocNone);
  //on cree un pixel white
  XColor white;
  white.red = 65535;
  white.green = 65535;
  white.blue = 65535;
  XAllocColor (display, color_map, &white);
  
  //on cree un pixel black
  XColor black;
  black.red = 0;
  black.green = 0;
  black.blue = 0;
  XAllocColor (display, color_map, &black);  

  //on cree un pixel rouge
  XColor red;
  red.red = 65535;
  red.green = 0;
  red.blue = 0;
  XAllocColor (display, color_map, &red);

  
  int x, y;
  time_t t;
  int has_finished = 0;
  while (!has_finished)  {
    XNextEvent(display, &report);
    switch  (report.type) {
    case Expose:   
      fprintf(stdout, "I have been exposed.\n");
      XPutImage(display, window, DefaultGC(display,0), xi, 0, 0, 0, 0, image->width, image->height);
      //on dessines les yeux
      XSetForeground (display, DefaultGC(display,0), white.pixel);
      XSetFillStyle (display, DefaultGC(display,0), FillSolid);
      XFillArc(display, window, DefaultGC(display,0), eye_left_x - eye_left_radius, 
	       eye_left_y - eye_left_radius, eye_left_radius * 2, eye_left_radius * 2, 0 * 64, 360 * 64);
      XFillArc(display, window, DefaultGC(display,0), eye_right_x - eye_right_radius, 
	       eye_right_y - eye_right_radius, eye_right_radius * 2, eye_right_radius * 2, 0 * 64, 360 * 64);
      //on dessine les pupilles
      XSetForeground (display, DefaultGC(display,0), black.pixel);
      XSetFillStyle (display, DefaultGC(display,0), FillSolid);
      XFillArc(display, window, DefaultGC(display,0), eye_left_x - eye_left_radius + eye_left_pupil_radius, 
	       eye_left_y - eye_left_radius + eye_left_pupil_radius, eye_left_radius * 2 - eye_left_pupil_radius * 2, 
	       eye_left_radius * 2 - eye_left_pupil_radius * 2, 0 * 64, 360 * 64);
      XFillArc(display, window, DefaultGC(display,0), eye_right_x - eye_right_radius + eye_right_pupil_radius, 
	       eye_right_y - eye_right_radius + eye_right_pupil_radius, eye_right_radius * 2 - eye_right_pupil_radius * 2, 
	       eye_right_radius * 2 - eye_right_pupil_radius * 2, 0 * 64, 360 * 64);
      XFlush(display);
      break;
    case KeyPress:
      if(lol_mode){
	time(&t);
	x = (int)(rand() / (double)RAND_MAX * ( window_width - image->width));
	y = (int)(rand() / (double)RAND_MAX * ( window_height - image->height));
	XSetForeground (display, DefaultGC(display,0), red.pixel);
	XSetFillStyle (display, DefaultGC(display,0), FillSolid);
	XFillRectangle (display, window, DefaultGC(display,0),
			0, 0, window_width, window_height); 
	XPutImage(display, window, DefaultGC(display,0), xi, 0, 
		  0, x, y, image->width, image->height);
	XFlush(display);
	printf("x %d y %d \n",x,y);
      }
      //Close the program if q is pressed.

      if (XLookupKeysym(&report.xkey, 0) == XK_q) {
	has_finished = 1;
      }
      break;
    case MotionNotify:
      if(move_mode){
	XPutImage(display, window, DefaultGC(display,0), xi, 0, 
		  0, 0, 0, image->width, image->height);

	//on dessine les yeux
	XSetForeground (display, DefaultGC(display,0), white.pixel);
	XSetFillStyle (display, DefaultGC(display,0), FillSolid);
	XFillArc(display, window, DefaultGC(display,0), eye_left_x - eye_left_radius, 
		 eye_left_y - eye_left_radius, eye_left_radius * 2, eye_left_radius * 2, 0 * 64, 360 * 64);
	XFillArc(display, window, DefaultGC(display,0), eye_right_x - eye_right_radius, 
		 eye_right_y - eye_right_radius, eye_right_radius * 2, eye_right_radius * 2, 0 * 64, 360 * 64);
      
	//on dessine les pupilles
	XSetForeground (display, DefaultGC(display,0), black.pixel);
	XSetFillStyle (display, DefaultGC(display,0), FillSolid);
      
	float sx, sy, ax, ay, px, py, r, R;
	sx = report.xmotion.x;
	sy = report.xmotion.y;
	//oeil de gauche
	r = eye_left_pupil_radius;
	R = eye_left_radius;
	ax = eye_left_x - r;
	ay = eye_left_y - r;
	px = ((sx - ax) * (R - r)) / 
	  sqrt( pow2(sx - ax) * pow2(sy - ay) ) + ax;
	py = ((sy - ay) * (R - r)) /
	  sqrt( pow2(sx - ax) * pow2(sy - ay) ) +  ay;
	printf("oeil gauche %d %d\n", (int)px, (int)py);
      
	XFillArc(display, window, DefaultGC(display,0), (int)px, (int)py, 
		 eye_left_radius * 2 - eye_left_pupil_radius * 2, 
		 eye_left_radius * 2 - eye_left_pupil_radius * 2, 0 * 64, 360 * 64);
      
	//oeil de droite
	r = eye_right_pupil_radius;
	R = eye_right_radius;
	ax = eye_right_x - r;
	ay = eye_right_y - r;
	px = (sx - ax) * (R - r) / 
	  sqrt( pow2(sx - ax) * pow2(sy - ay) ) + ax;
	py = (sy - ay) * (R - r) /
	  sqrt( pow2(sx - ax) * pow2(sy - ay) ) +  ay;
	printf("oeil droit %d %d\n", (int)px, (int)py);
      
	XFillArc(display, window, DefaultGC(display,0), (int)px, (int)py, 
		 eye_right_radius * 2 - eye_right_pupil_radius * 2, 
		 eye_right_radius * 2 - eye_right_pupil_radius * 2, 0 * 64, 360 * 64);
      
	XFlush(display);
      }
      break;
    default:
      printf("An event has occured\n");
      break;
    }
  }

  //on ferme le display
  XCloseDisplay(display);
  return 0;
}



