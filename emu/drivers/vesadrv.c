#include <stdio.h>
#include <stdlib.h>
#include <vesadrv.h>
#include <SDL/SDL.h>
#include <SDL/SDL_mutex.h>

extern SDL_Surface *screen;
extern SDL_mutex *mutex;

Uint32 libtacos_refresh_screen(Uint32 interval, void *param) {
  vesa_flush();
  return interval;
}

void vesa_set_mode(struct vesa_setmode_req* mode) {
  int width = mode->width;
  int height = mode->height;
  int bpp = mode->bpp;

  printf("Initializing VESA (%d*%d*%d)...\n", width, height, bpp);

  SDL_mutexP(mutex);
  screen = SDL_SetVideoMode(width, height, bpp, SDL_SWSURFACE);
  SDL_mutexV(mutex);
  if (screen == NULL) {
    fprintf(stderr, "Impossible de passer en %d*%d*%d : %s\n", width, height, bpp, SDL_GetError());
    exit(1);
  }
  
  SDL_AddTimer(40, libtacos_refresh_screen, NULL);
}

void vesa_flush() {
  SDL_mutexP(mutex);
  SDL_UpdateRect(screen, 0, 0, screen->w, screen->h);
  SDL_mutexV(mutex);
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
