/**
 * gcc -Wall -shared hooklib.c -o hooklib.so -ldl -lSDL
 * gcc -Wall zen_graph.c -o zen_graph
 * LD_PRELOAD=./hooklib.so ./zen_graph
 */
#define _GNU_SOURCE
#include <dlfcn.h>
#include <stdio.h>
#include <SDL/SDL.h>
#include <unistd.h>
#include <signal.h>
#include "vga_modes.h"

int (*libc_open)(const char *pathname, int flags);
int (*libc_close)(int fd);
int (*libc_ioctl)(int fd, unsigned long request, void *data);

void alarm_handler(int signum) {
	if (SDL_QuitRequested()) {
		exit(0);
	}

	alarm(1);
}

void __attribute__((constructor)) init() {
	printf("Initializing hooklib.\n");
	libc_open = dlsym(RTLD_NEXT, "open");
	libc_close = dlsym(RTLD_NEXT, "close");
	libc_ioctl = dlsym(RTLD_NEXT, "ioctl");

	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_NOPARACHUTE) < 0) {
		fprintf(stderr, "Vous n'avez pas compris les concepts : %s\n", SDL_GetError());
		exit(1);
	}

	signal(SIGALRM, alarm_handler);
	alarm(1);
}

void __attribute__((destructor)) destroy() {
	printf("Destroying hooklib.\n");
	SDL_Quit();
}

SDL_Surface *screen;
int vga_width, vga_height, vga_bpp;

void vga_set_mode(enum vga_mode mode) {
	vga_width = vga_modes[mode].width;
	vga_height = vga_modes[mode].height;
	vga_bpp = vga_modes[mode].bpp;

	printf("Initializing VGA (%d*%d*%d)...\n", vga_width, vga_height, vga_bpp);

	screen = SDL_SetVideoMode(vga_width, vga_height, vga_bpp*8, SDL_SWSURFACE);
	if (screen == NULL) {
		fprintf(stderr, "Impossible de passer en %d*%d*%d : %s\n", vga_width, vga_height, vga_bpp, SDL_GetError());
		exit(1);
	}	
}

void vga_write_buf(char *buffer) {
	memcpy(screen->pixels, buffer, vga_width * vga_height * vga_bpp);
	SDL_UpdateRect(screen, 0, 0, vga_width, vga_height);
}

int open(const char *pathname, int flags) {
	printf("open!\n");
	//return libc_open(pathname, flags);
	return 0;
}

int close(int fd) {
	printf("close!\n");
	//return libc_close(fd);
	return 0;
}

int ioctl(int fd, unsigned long request, void *data) {
	//printf("ioctl!\n");
	//return libc_ioctl(fd, request, data);
	switch (request) {
		case SETMODE: {
			enum vga_mode mode = (enum vga_mode)data;
			vga_set_mode(mode);
			return 0;
		}
		case FLUSH: {
			char *user_buffer = (char*)data;
			vga_write_buf(user_buffer);
			return 0;
		}
		default:
			return -1;
	}
}
