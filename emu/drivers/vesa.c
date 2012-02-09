#include <vesa.h>
#include <SDL/SDL.h>

extern SDL_Surface *screen;

void vesa_set_mode(struct vesa_setmode_req* mode) {
	int width = mode->width;
	int height = mode->height;
	int bpp = mode->bpp;

	printf("Initializing VESA (%d*%d*%d)...\n", width, height, bpp);

	screen = SDL_SetVideoMode(width, height, bpp, SDL_SWSURFACE);
	if (screen == NULL) {
		fprintf(stderr, "Impossible de passer en %d*%d*%d : %s\n", width, height, bpp, SDL_GetError());
		exit(1);
	}
}

void vesa_flush() {
	SDL_UpdateRect(screen, 0, 0, screen->w, screen->h);
	memset(screen->pixels, 0, screen->w * screen->h * screen->format->BytesPerPixel);
}

int vesa_ioctl(unsigned long request, void *data) {
	int ret;

	switch (request) {
		case SETMODE: {
			struct vesa_setmode_req* mode = (struct vesa_setmode_req*)data;
			vesa_set_mode(mode);
			ret = 0;
			break;
		}
		case GETVIDEOADDR: {
			*((void**)data) = screen->pixels;
			ret = 0;
			break;
		}
		case FLUSH: {
			vesa_flush();
			ret = 0;
			break;
		}
		default: {
			ret = -1;
		}
	}

	return ret;
}
