#include <clibtacos>
#include <fcntl.h>
#include <mouse.h>
#include <mouse_types.h>
#include <screen.h>
#include <string.h>
#include <unistd.h>
#include <window.h>

Mouse::Mouse() {
  this->fd = open("/dev/mouse", 0);
}

void Mouse::checkEvents() {
  // Get the mouse state
  mousestate_t state;
  memset(&state, 0, sizeof(mousestate_t));
  read(this->fd, &state, 0);

  Screen *screen = Screen::getInstance();

  if (screen->getMouseX() != state.x || screen->getMouseY() != state.y) {
    screen->setMouseX(state.x);
    screen->setMouseY(state.y);

    // TODO : detect the window which is focused
    // TODO : detect here real selected window ...
    // computing coordinates
    // XXX : We get the first non root window
    if (screen->windows.size() > 1) {
      Window *focused = screen->windows[1];
      int x = state.x - focused->x;
      int y = state.y - focused->y;

      // send th event if the the pointer is in the current focused window
      if (x >= 0 && y >= 0 && x < focused->width && y < focused->height) {
        debug("Position du pointeur %d %d, focused window %d %d, width height %d %d \n",x, y, focused->x, focused->y, focused->width, focused->height);

        // it's time to send mouse Event
        EventPointerMoved pointerMoved(focused->id, x, y, state.x, state.y);
        focused->deliverDeviceEvent(&pointerMoved, sizeof(pointerMoved));
      }
    }
  }
}
