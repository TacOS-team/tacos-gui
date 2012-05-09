/**
 * @file mouse.cpp
 * Mouse class implementation.
 */

#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include <sys/time.h>

#include <libtacos.h>
#include <mouse.h>
#include <screen.h>
#include <window.h>

Mouse* Mouse::instance = NULL;

Mouse::Mouse() {
  this->fd = open("/dev/mouse", 0);
  this->pointerBackupX = 0;
  this->pointerBackupY = 0;
  Screen *screen = Screen::getInstance();
  this->pointer = new Pixmap(screen, 1, NULL, PRON_MOUSE_POINTER_WIDTH, PRON_MOUSE_POINTER_HEIGHT, screen->bitsPerPixel);
  this->pointerBackup = new Pixmap(screen, 2, NULL, PRON_MOUSE_POINTER_WIDTH, PRON_MOUSE_POINTER_HEIGHT, screen->bitsPerPixel);
  this->pointerHidden = true;

  this->mouseX = 0;
  this->mouseY = 0;
  this->mouseB1 = false;
  this->mouseB2 = false;
  this->mouseB3 = false;
  this->mouseB4 = false;
  this->mouseB5 = false;
  this->mouseB6 = false;

  this->lastMouseEvent = 0;
  this->lastSentX = 0;
  this->lastSentY = 0;

  /** @todo xxx red is transparent */
  color_t oldFg = screen->getGC()->fg;
  COLOR(screen->getGC()->fg, 24).r = 255;
  COLOR(screen->getGC()->fg, 24).g = 0;
  COLOR(screen->getGC()->fg, 24).b = 0;

  this->pointer->fillRectangle(0, 0, PRON_MOUSE_POINTER_WIDTH, PRON_MOUSE_POINTER_HEIGHT);

  screen->getGC()->fg = oldFg;

  this->pointer->drawLine(0, 0, 0, PRON_MOUSE_POINTER_HEIGHT - 1);
  this->pointer->drawLine(0, 0, PRON_MOUSE_POINTER_WIDTH - 1, 0);
}

void Mouse::checkEvents() {
  // Get the mouse state
  mousestate_t state;
  memset(&state, 0, sizeof(mousestate_t));
  read(this->fd, &state, sizeof(mousestate_t));

  // Handle motion mouse events
  this->handleMotion(&state);

  // Handle button mouse events
  this->handleButton(&state);
}

void Mouse::handleMotion(mousestate_t *state) {
  Screen *screen = Screen::getInstance();

  // Has pointer moved since the previous select ?
  if (this->mouseX != state->x || this->mouseY != state->y) {
    // Set the new coordinates in the screen
    this->mouseX = state->x;
    this->mouseY = state->y;

    // Move the pointer
    this->hidePointer();
    this->showPointer();

    // We have to recompute the mouseWin
    this->updateMouseWin();
    this->updateFocusWin();
  }

  if (this->lastSentX != this->mouseX || this->lastSentY != this->mouseY) {
    struct timeval now;
    gettimeofday(&now, NULL);
    unsigned long now_ms = now.tv_sec * 1000 + now.tv_usec/1000;
    if (now_ms - this->lastMouseEvent > 60) {
      // Send the event to the current mouseWin
      Window *mouseWin = screen->getMouseWin();
      
      int x = this->mouseX - this->lastSentX;
      int y = this->mouseY - this->lastSentY;
      
      EventPointerMoved pointerMoved(mouseWin->getId(), x, y, state->x, state->y);
      mouseWin->deliverDeviceEvent(&pointerMoved, sizeof(pointerMoved));

      this->lastMouseEvent = now_ms;
      this->lastSentX = this->mouseX;
      this->lastSentY = this->mouseY;
    }
  }
}

void Mouse::handleButton(mousestate_t *state) {
  // We have to compare the state of each buttons and if one button has changed, send the event
  if (this->mouseB1 != state->b1 ||
      this->mouseB2 != state->b2 ||
      this->mouseB3 != state->b3 ||
      this->mouseB4 != state->b4 ||
      this->mouseB5 != state->b5 ||
      this->mouseB6 != state->b6 ) {

    Screen *screen = Screen::getInstance();
    //debug("Mouse button state %d %d %d %d %d %d \n", state->b1, state->b2, state->b3, state->b4, state->b5, state->b6);

    this->mouseB1 = state->b1;
    this->mouseB2 = state->b2;
    this->mouseB3 = state->b3;
    this->mouseB4 = state->b4;
    this->mouseB5 = state->b5;
    this->mouseB6 = state->b6;

    // We get the window which consairns the event
    Window *mouseWin = screen->getMouseWin();

    // Computing the relative coordinates
    int relX = state->x - mouseWin->getX();
    int relY = state->y - mouseWin->getY();

    // delivers the event
    EventMouseButton mouseButton(mouseWin->getId(), state->b1, state->b2,
                                 state->b3, state->b4, state->b5, state->b6,
                                 relX, relY, state->x, state->y);
    mouseWin->deliverDeviceEvent(&mouseButton, sizeof(mouseButton));
  }
}

void Mouse::updateMouseWin() {
  Screen *screen = Screen::getInstance();
  // We have to explorate window tree from the  screen to his children
  // and check if the pointer is inside them : We can stop when we have found one window
  // because it is necessarily the one which is drawn
  Window *currentWin = screen->getRoot();
  // We stop when currentWin has no children 
  while (currentWin->lastChild != NULL) {
    // we have to run throug the childs from the right to the left
    // and take the first (the one with the bigger Z)
    Window *currentChild = currentWin->lastChild;
    while (currentChild != NULL) {
      if (currentChild->realized() && currentChild->contains(this->mouseX, this->mouseY)) {
        currentWin = currentChild;
        break;// We have found the first containing child we have to stop
      } 
      currentChild = currentChild->prevSibling;
    }
    if (currentChild == NULL) {
      break;// We have no child which contains the pointer we can stop
    }
  }

  // MouseWin is computed we can store it in the screen to increzse performances
  screen->setMouseWin(currentWin);
}

void Mouse::updateFocusWin() {
  Screen *screen = Screen::getInstance();
  Window *newFocusWin = NULL;
  unsigned int eventMask = PRON_EVENTMASK(EV_KEY_PRESSED);

  Window *w = screen->getMouseWin();
  while (w != NULL) {
    if (w->acceptsEvents(eventMask)) {
      newFocusWin = w;
      break;
    } else if (w == screen->getRoot() || (w->dontPropagateMask & eventMask)) {
      newFocusWin = screen->getRoot();
      break;
    }
    w = w->parent;
  }

  screen->setFocusWin(newFocusWin);
}

bool Mouse::overlapsPointer(int x1, int y1, int x2, int y2) {
  return !(x1 >= this->mouseX + PRON_MOUSE_POINTER_WIDTH ||
      y1 >= this->mouseY + PRON_MOUSE_POINTER_HEIGHT ||
      x2 < this->mouseX || y2 < this->mouseY);
}

void Mouse::hidePointer() {
  if (!this->pointerHidden) {
    Screen *screen = Screen::getInstance();

    for (int y = 0; y < PRON_MOUSE_POINTER_HEIGHT; y++) {
      for (int x = 0; x < PRON_MOUSE_POINTER_WIDTH; x++) {
        void *destination = screen->getRoot()->pixelAddr(x + this->pointerBackupX, y + this->pointerBackupY);
        void *source = this->pointerBackup->pixelAddr(x, y);
        memcpy(destination, source, screen->bytesPerPixel); 
      }
    }
    
    this->pointerHidden= true;
  }
}

void Mouse::showPointer() {
  if (this->pointerHidden) {
    Screen *screen = Screen::getInstance();

    // Save the pointer background and draw the pointer
    this->pointerBackupX = this->mouseX;
    this->pointerBackupY = this->mouseY;

    for (int y = 0; y < PRON_MOUSE_POINTER_HEIGHT; y++) {
      for (int x = 0; x < PRON_MOUSE_POINTER_WIDTH; x++) {
        // Backup old pixel
        void *destination = this->pointerBackup->pixelAddr(x, y);
        void *source = screen->getRoot()->pixelAddr(x + this->mouseX, y + this->mouseY);
        memcpy(destination, source, screen->bytesPerPixel);

        // Draw new pixel
        /** @todo xxx red is transparent haha degueu */
        if (this->pointer->getPixel(x, y) != 0x00FF0000) {
          destination = source;
          source = this->pointer->pixelAddr(x, y);
          memcpy(destination, source, screen->bytesPerPixel);
        }
      }
    }

    this->pointerHidden = false;
  }
}

Mouse* Mouse::getInstance() {
  if (Mouse::instance == NULL) {
    Mouse::instance = new Mouse();
  }
  return Mouse::instance;
}

int Mouse::getMouseX() {
  return this->mouseX;
}

int Mouse::getMouseY() {
  return this->mouseY;
}

bool Mouse::getMouseB1() {
  return this->mouseB1;
}

bool Mouse::getMouseB2() {
  return this->mouseB2;
}

bool Mouse::getMouseB3() {
  return this->mouseB3;
}

bool Mouse::getMouseB4() {
  return this->mouseB4;
}

bool Mouse::getMouseB5() {
  return this->mouseB5;
}

bool Mouse::getMouseB6() {
  return this->mouseB6;
}
