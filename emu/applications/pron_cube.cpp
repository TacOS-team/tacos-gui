#include <cstdlib>
#include <pronlib.h>
#include <unistd.h>
#include <cstdio>

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

void draw_cube(Display *d, Window w) {
  int ligne;
  
  for (ligne = 0; ligne < 12; ligne++) {
    if (!pronDrawLine(d, w, 0, 
        (int)(cube[lignes[ligne][0]][0]*50+150),
        (int)(cube[lignes[ligne][0]][1]*50+100),
        (int)(cube[lignes[ligne][1]][0]*50+150),
        (int)(cube[lignes[ligne][1]][1]*50+100))) {
      fprintf(stderr, "pron has closed the connection.\n");
      exit(1);
    }
  }
}

int main(int argc, char *argv[]) {
  int x, y;

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

  Window w = pronCreateWindow(d, d->rootWindow, x, y, 320, 240);

  while (1) {
    pronClearWindow(d, w);  
    int i = 0;
    for (i = 0; i < 8; i++) {
      rotate_point(cube[i]);
    }
    draw_cube(d, w);
    usleep(20000);
  }

  pronDisconnect(d);

  return 0;
}
