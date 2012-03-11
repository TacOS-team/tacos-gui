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

  // Has pointer moved since the previous select ?
  if (screen->getMouseX() != state.x || screen->getMouseY() != state.y) {
    // Set the new coordinates in the screen
    screen->setMouseX(state.x);
    screen->setMouseY(state.y);

    Window *mouseWin = screen->getMouseWin();

    // We have to recompute the mouseWin
    // We have to explorate window tree from the root to his children
    // and check if the pointer is inside them : We can stop when we have found one window
    // because it is necessarily the one which is drawn
    Window *currentWin = screen->getRoot();
    // We stop when currentWin has no children 
    while(currentWin->lastChild != NULL){
      // we have to run throug the childs from the right to the left
      // and take the first (the one with the bigger Z)
      Window *currentChild = currentWin->lastChild;
      while(currentChild != NULL){
        if(currentChild->contains(state.x, state.y)){
          currentWin = currentChild;
          break;// We have found the first containing child we have to stop
        }
        currentChild = currentChild->prevSibling;
      }
      if(currentChild == NULL){
        break;// We have no child which contains the pointer we can stop
      }
    }
    mouseWin = currentWin;// We have found the nwe mouseWin
  
    // MouseWin is computed we can store it in the screen to increse performances
    screen->setMouseWin(mouseWin);

    //computing relative coordinates 
    int x = state.x - mouseWin->x;
    int y = state.y - mouseWin->y;
    // send th event if the the pointer is in the current mouseWin window
    if (x >= 0 && y >= 0 && x < mouseWin->width && y < mouseWin->height) {
      debug("Position du pointeur %d %d, mouseWin window %d %d, width height %d %d \n",x, y, mouseWin->x, mouseWin->y, mouseWin->width, mouseWin->height);

      // it's time to send mouse Event
      EventPointerMoved pointerMoved(mouseWin->id, x, y, state.x, state.y);
      mouseWin->deliverDeviceEvent(&pointerMoved, sizeof(pointerMoved));
    }

  }
}
