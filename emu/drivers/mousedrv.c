#include <mousedrv.h>
#include <string.h>
#include <SDL/SDL_mutex.h>

extern SDL_Surface *screen;
extern SDL_mutex *mutex;

ssize_t mouse_read(void *buf) {
  mousestate_t mouse_state;

  SDL_mutexP(mutex);
  SDL_PumpEvents();
  SDL_mutexV(mutex);
  int mouse_x, mouse_y;
  uint8_t buttons = SDL_GetMouseState(&mouse_x, &mouse_y);
  mouse_state.x = mouse_x;
  mouse_state.y = mouse_y;
  mouse_state.b1 = buttons & SDL_BUTTON(1);
  mouse_state.b2 = buttons & SDL_BUTTON(2);
  mouse_state.b3 = buttons & SDL_BUTTON(3);
  mouse_state.b4 = buttons & SDL_BUTTON(4);
  mouse_state.b5 = buttons & SDL_BUTTON(5);
  mouse_state.b6 = buttons & SDL_BUTTON(6);

  memcpy(buf, &mouse_state, sizeof(mouse_state));

  return sizeof(mouse_state);
}

int mouse_ioctl(unsigned long request, void *data) {
  int ret;

  switch (request) {
    default: {
      ret = -1;
    }
  }

  return ret;
}
