/**
 * @file clock_graph.c
 *
 * @author TacOS developers 
 *
 * @section LICENSE
 *
 * Copyright (C) 2011 - TacOS developers.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 3 of
 * the License, or (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details at
 * http://www.gnu.org/copyleft/gpl.html
 *
 * You should have received a copy of the GNU General Public License 
 * along with this program; if not, see <http://www.gnu.org/licenses>.
 *
 * @section DESCRIPTION
 *
 * Very basic pong game
 */

#include <cstdlib>
#include <cstdio>
#include <unistd.h>
#include <pronlib.h>
#include <libtacos.h>       
#include <string.h>

#define WIDTH 320
#define HEIGHT 200

#define BALL_SIZE 7
#define RACKET_WIDTH 6
#define RACKET_HEIGHT 50

void moveBall(int * x, int * y, int * velX, int * velY, int racketY) {
  *x += *velX;
  *y += *velY;

  // check collisions with right wall
  if (*x + BALL_SIZE >= WIDTH)
    *velX = -*velX;
  // check collisions with racket/left wall
  else if (*x <= RACKET_WIDTH) {
    if(! (*y + BALL_SIZE >= racketY && *y <= racketY + RACKET_HEIGHT)){
      usleep(1000000);
      *x = WIDTH/2 - BALL_SIZE / 2;
      *y = HEIGHT/2 - BALL_SIZE /2;
      *velX = -*velX;
    }
    else {
      *velX = -*velX;
    }
  }

  // check collisions with top/bottom walls
  if (*y <= 0 || *y + BALL_SIZE >= HEIGHT)
    *velY = -*velY;
}

int main() {
  
  int ballX = WIDTH/2 - BALL_SIZE / 2;
  int ballY = HEIGHT/2 - BALL_SIZE /2;
  int velX = 5;
  int velY = 5;
  int racketY = 0;

  Display *d = pronConnect();
  if (d == NULL) {
    fprintf(stderr, "Unable to connect to pron.\n");
    exit(1);
  }

  Window w = pronCreateWindow(d, d->rootWindow, 0, 0, WIDTH, HEIGHT);

  // On récupère le contexte graphique par défaut
  GC gc = d->defaultGC;

  // We've to map the window
  pronMapWindow(d, w);

  // S'abonne aux évènements
  pronSelectInput(d, w, PRON_EVENTMASK(EV_POINTER_MOVED) | PRON_EVENTMASK(EV_MOUSE_BUTTON));

  // On cree un énènement
  PronEvent *e = getPronEvent();

  //On cree une fake image
  int x = 1, y = 1, width = 6, height = 6, depth = 24;
  char * image_data = (char *)malloc(width * width * depth);
  PronImage image(width, height, ZPixmap, image_data, depth);

  while (1) {

    //On récupère un évènement
    if (!pronNextEvent(d, e)) {
      fprintf(stderr, "pron has closed the connection.\n");
      exit(1);
    }

    switch (e->type) {
    case EV_POINTER_MOVED: {
      EventPointerMoved *pointerMoved = (EventPointerMoved*) e;
      // Récupération de la coordonnée Y de la souris pour la raquette
      racketY = pointerMoved->y;
      break;
    }
    default:
      break;
    }

    pronClearWindow(d, w);  
    // TODO: draw ball and racket
    moveBall(&ballX,&ballY,&velX,&velY,racketY);
    // draw the ball
    pronFillRectangle(d, w, gc, ballX, ballY, BALL_SIZE, BALL_SIZE);
    // drax the racket
    pronFillRectangle(d, w, gc, 0, racketY, RACKET_WIDTH, RACKET_HEIGHT);
    usleep(10000);

    //on envoie une sous image de l'image dans la fenetre
    pronPutImage(d, w, gc, &image, x, y, 0, 0, width - 1, height - 1);
  }

  pronDisconnect(d);
  
  return 0;
}
