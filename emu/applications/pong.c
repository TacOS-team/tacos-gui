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

#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <sys/ioctl.h>
#include <vga_types.h>
#include <mouse_types.h>

#define WIDTH 320
#define HEIGHT 200

#define BALL_SIZE 7
#define RACKET_WIDTH 6
#define RACKET_HEIGHT 50

#define _COLOR(_p, _bpp) _p.bpp ## _bpp
#define COLOR(_p, _bpp) _COLOR(_p, _bpp)

static char buffer[WIDTH*HEIGHT];

static int vga_fd;

void clear_buffer(char* buffer) {
	memset(buffer, 0, WIDTH*HEIGHT); 
}

void put_pixel(char color, int x, int y) {
    if (x >= 0 && x < WIDTH && y >= 0 && y < HEIGHT) {
	buffer[x+y*WIDTH] = color;
    }
}

void drawFilledRect(int x1, int y1,int x2, int y2, char color) {
    int i = x1;
    int j = y1;
    for (j = y1; j<y2; j++) {
	for (i = x1; i<x2; i++) {
	    put_pixel(color,i,j);
	}
    }
}

void drawBall (int x, int y, char color) {
    drawFilledRect (x,y,x + BALL_SIZE, y + BALL_SIZE,color);
}

void drawRacket (int y, char color) {
    if (y > HEIGHT - RACKET_HEIGHT) {
	y =  HEIGHT - RACKET_HEIGHT;
    }
    drawFilledRect(0,y,RACKET_WIDTH, y + RACKET_HEIGHT,color);
}

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
    vga_fd = open("/dev/vga", O_RDONLY);
    ioctl(vga_fd, SETMODE, (void*)vga_mode_320x200x256); 

    int fd = open("/dev/mouse", O_RDONLY);
    mousestate_t data;

    int ballX = WIDTH/2 - BALL_SIZE / 2;
    int ballY = HEIGHT/2 - BALL_SIZE /2;
    int velX = 10;
    int velY = 10;

    char color = 1;

    while (1) {
	clear_buffer(buffer);

	// read mouse position
	read(fd, &data, sizeof(data));
	data.y = 199 - data.y;
	data.x = data.x * WIDTH / 320;
	data.y = data.y * HEIGHT / 200;

	// draw ball and racket
	moveBall(&ballX,&ballY,&velX,&velY,data.y);
	drawBall(ballX,ballY,color);
	drawRacket(data.y,color);
	ioctl(vga_fd, FLUSH, buffer);
	
	usleep(10000);
    }
    return 0;
}
