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

/*
  #define BALL_SIZE 7
  #define RACKET_WIDTH 6
  #define RACKET_HEIGHT 50
*/

#define BALL_SIZE 1
#define RACKET_WIDTH 1
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
  int velX = 10;
  int velY = 10;

  Display *d = pronConnect();
  if (d == NULL) {
    fprintf(stderr, "Unable to connect to pron.\n");
    exit(1);
  }

  Window w = pronCreateWindow(d, d->rootWindow, 0, 0, WIDTH, HEIGHT);

  while (1) {
    // read mouse position
    //TODO: ask pronlib the mouse position

    pronClearWindow(d, w);	
    //TODO: draw ball and racket
    //moveBall(&ballX,&ballY,&velX,&velY,NULL);
    usleep(10000);

  }

  pronDisconnect(d);
  
  return 0;
}
