#include <kbddrv.h>
#include <stdio.h>
#include <string.h>
#include <SDL/SDL_mutex.h>

extern SDL_Surface *screen;
extern SDL_mutex *mutex;

ssize_t kbd_read(void *buf) {
  int ret;

  SDL_mutexP(mutex);
  SDL_PumpEvents();
  SDL_mutexV(mutex);
  SDL_Event event;
  if (SDL_PeepEvents(&event, 1, SDL_GETEVENT, SDL_KEYDOWNMASK | SDL_KEYUPMASK)) {
    if (event.key.state == SDL_PRESSED && event.key.keysym.sym == SDLK_F11 &&
        (event.key.keysym.mod & KMOD_ALT)) {
      SDL_WM_ToggleFullScreen(screen);
      ret = EOF;
    } else {
      kbdstate_t kbd_state;
      kbd_state.state = event.key.state;
      kbd_state.keysym = event.key.keysym.sym;
      kbd_state.modifiers = event.key.keysym.mod;
      kbd_state.ch = event.key.keysym.unicode;

      memcpy(buf, &kbd_state, sizeof(kbd_state));
      ret = sizeof(kbd_state);
    }
  } else {
    ret = EOF;
  }

  return ret;
}

int kbd_ioctl(unsigned long request, void *data __attribute__((unused))) {
  int ret;

  switch (request) {
    default: {
      ret = -1;
    }
  }

  return ret;
}
