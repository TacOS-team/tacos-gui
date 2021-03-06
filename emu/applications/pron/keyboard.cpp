#include <clibtacos>
#include <fcntl.h>
#include <keyboard.h>
#include <kbd_types.h>
#include <screen.h>
#include <string.h>
#include <unistd.h>
#include <window.h>

Keyboard::Keyboard() {
  this->fd = open("/dev/keyboard", 0);
}

void Keyboard::checkEvents() {
  Screen *screen = Screen::getInstance();

  // Get the keyboard state
  kbdstate_t state;
  memset(&state, 0, sizeof(kbdstate_t));

  if (read(this->fd, &state, 0) > 0) {
    // TODO: compute real source window
    Window *src = screen->windows[0];
    if (state.state == PRON_RELEASED) {
      EventKeyReleased keyReleased(src->id, src->id, 0, 0, 0, 0, state.keysym, state.modifiers);
      src->deliverDeviceEvent(&keyReleased, sizeof(keyReleased));
    } else if (state.state == PRON_PRESSED) {
      EventKeyPressed keyPressed(src->id, src->id, 0, 0, 0, 0, state.keysym, state.modifiers);
      src->deliverDeviceEvent(&keyPressed, sizeof(keyPressed));
    }
  }
}