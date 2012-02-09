#include <vga.h>
#include <vga_modes.h>
#include <SDL/SDL.h>

extern SDL_Surface *screen;

void vga_set_mode(enum vga_mode mode) {
	int width = vga_modes[mode].width;
	int height = vga_modes[mode].height;
	int bpp = vga_modes[mode].bpp;

	printf("Initializing VGA (%d*%d*%d)...\n", width, height, bpp);

	screen = SDL_SetVideoMode(width, height, bpp*8, SDL_SWSURFACE);
	if (screen == NULL) {
		fprintf(stderr, "Impossible de passer en %d*%d*%d : %s\n", width, height, bpp, SDL_GetError());
		exit(1);
	}
}

void vga_write_buf(char *buffer) {
	memcpy(screen->pixels, buffer, screen->w * screen->h * screen->format->BytesPerPixel);
	SDL_UpdateRect(screen, 0, 0, screen->w, screen->h);
}

int vga_ioctl(unsigned long request, void *data) {
	int ret;

	switch (request) {
		case SETVGAMODE: {
			enum vga_mode mode = (enum vga_mode)data;
			vga_set_mode(mode);
			ret = 0;
			break;
		}
		case FLUSHVGA: {
			char *user_buffer = (char*)data;
			vga_write_buf(user_buffer);
			ret = 0;
			break;
		}
		default: {
			ret = -1;
		}
	}

	return ret;
}
