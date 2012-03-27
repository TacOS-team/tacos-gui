#include <cstdlib>
#include <pronlib.h>
#include <unistd.h>
#include <cstdio>
#include <pronlib_structs.h>
#include <pronlib_enums.h>

float cube[][3]= {
  {  1.0f,  1.0f, -1.0f },
  {  1.0f, -1.0f, -1.0f },
  { -1.0f, -1.0f, -1.0f },
  { -1.0f,  1.0f, -1.0f },
  {  1.0f,  1.0f,  1.0f },
  {  1.0f, -1.0f,  1.0f },
  { -1.0f, -1.0f,  1.0f },
  { -1.0f,  1.0f,  1.0f }
};

int lignes[][2] = {
  {0, 1}, // face du haut
  {1, 2},
  {2, 3},
  {3, 0},
  {4, 5}, // face du bas
  {5, 6},
  {6, 7},
  {7, 4},
  {0, 4}, // Arrêtes des cotés
  {1, 5},
  {2, 6},
  {3, 7}
};

unsigned short int width, height;
unsigned short int cubeWidth;
unsigned short int xCubeShift, yCubeShift;

void updateZoom(unsigned short int width, unsigned short int height) {
  cubeWidth = ((width < height) ? width : height)/4;
  xCubeShift = 2*cubeWidth + (width  - cubeWidth*4)/2;
  yCubeShift = 2*cubeWidth + (height - cubeWidth*4)/2;
}

void rotate_point(float point[3]) {
  float temp[3];
  // theta = 4°
  float cos_theta =   0.99984769f;
  float sin_theta =   0.01745240f;

  // phi = 1°
  float cos_phi =   0.99756405f;
  float sin_phi =   0.06975647f;

  temp[0] = point[0];
  temp[1] = point[1]*cos_theta - point[2]*sin_theta;
  temp[2] = point[1]*sin_theta + point[2]*cos_theta;

  point[0] = temp[0]*cos_phi - temp[2]*sin_phi;
  point[1] = temp[1];
  point[2] = temp[0]*sin_phi + temp[2]*cos_phi;
}

void draw_cube(Display *d, Pixmap p, Window w) {
  int ligne;

  for (ligne = 0; ligne < 12; ligne++) {
    if (!pronDrawLine(d, p, d->defaultGC,
	     (int)(cube[lignes[ligne][0]][0]*cubeWidth+xCubeShift),
	     (int)(cube[lignes[ligne][0]][1]*cubeWidth+yCubeShift),
	     (int)(cube[lignes[ligne][1]][0]*cubeWidth+xCubeShift),
	     (int)(cube[lignes[ligne][1]][1]*cubeWidth+yCubeShift))) {
      fprintf(stderr, "pron has closed the connection.\n");
      exit(1);
    }
  }

  pronCopyArea(d, p, w, d->defaultGC, 0, 0, width, height, 0, 0);
}

int main(int argc, char *argv[]) {
  int x, y;
  width  = 200;
  height = 200;

  if (argc > 2) {
    x = atoi(argv[1]);
    y = atoi(argv[2]);
    printf("Position: %d %d\n", x, y);
  } else {
    x = 20;
    y = 10;
  }

  Display *d = pronConnect();
  if (d == NULL) {
    fprintf(stderr, "Unable to connect to pron.\n");
    exit(1);
  }

  updateZoom(width, height);

  Window w = pronCreateWindow(d, d->rootWindow, x, y, width, height);
  pronSelectInput(d, w, PRON_EVENTMASK(EV_DESTROY_WINDOW) | PRON_EVENTMASK(EV_RESIZE_WINDOW));

  Pixmap p = pronCreatePixmap(d, 1024, 768, 24);

  color_t bgColor;
  COLOR(bgColor, 24).r = (w >> 16) << 3;
  COLOR(bgColor, 24).g = (w >> 16) << 3;
  COLOR(bgColor, 24).b = (w >> 16) << 3;

  PronGCValues values;
  values.fg = bgColor;
  GC bgGC = pronCreateGC(d, values, GC_VAL_FG);
  
  PronWindowAttributes newAttr;
  newAttr.bgColor = bgColor;
  pronSetWindowAttributes(d, w, newAttr, WIN_ATTR_BG_COLOR);

  pronMapWindow(d, w);

  PronEvent *event = getPronEvent();

  while (1) {
    while (pronNextEvent(d, event, true)) {
      switch (event->type) {
        case EV_DESTROY_WINDOW : {
          EventDestroyWindow *destroyWindowEvent = (EventDestroyWindow*) event;
          printf("DestroyWindow event received for %d\n", destroyWindowEvent->window);
          if (destroyWindowEvent->window == w) {
            pronDisconnect(d);
          }
          return 0;
          break;
        }
        case EV_RESIZE_WINDOW : {
          EventResizeWindow *resizeWindowEvent = (EventResizeWindow*) event;
          width = resizeWindowEvent->width;
          height = resizeWindowEvent->height;
          updateZoom(width, height);
          break;
        }
        default:
        break;
      }
    }

    pronFillRectangle(d, p, bgGC, 0, 0, width, height);

    for (int i = 0; i < 8; i++) {
      rotate_point(cube[i]);
    }

    draw_cube(d, p, w);

    usleep(50000);
  }

  pronFreePixmap(d, p);
  pronDestroyWindow(d, w);

  pronDisconnect(d);

  return 0;
}
