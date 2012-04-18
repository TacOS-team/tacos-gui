#include <fcntl.h>
#include <string.h>
#include <unistd.h>

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
  this->pointerBackup = (char *)malloc(PRON_MOUSE_POINTER_WIDTH * PRON_MOUSE_POINTER_HEIGHT * screen->bytesPerPixel);
  memset(this->pointerBackup, 0, PRON_MOUSE_POINTER_WIDTH * PRON_MOUSE_POINTER_HEIGHT * screen->bytesPerPixel);
  this->pointerBackuped = false;

  this->mouseX = 0;
  this->mouseY = 0;
  this->mouseB1 = false;
  this->mouseB2 = false;
  this->mouseB3 = false;
  this->mouseB4 = false;
  this->mouseB5 = false;
  this->mouseB6 = false;
}

void Mouse::checkEvents() {
  // Get the mouse state
  mousestate_t state;
  memset(&state, 0, sizeof(mousestate_t));
  read(this->fd, &state, 0);

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

    // We have to recompute the mouseWin
    this->updateMouseWin();
    Window *mouseWin = screen->getMouseWin();

    //computing relative coordinates 
    int x = state->x - mouseWin->x;
    int y = state->y - mouseWin->y;
    
    // send the event to the current mouseWin
    //debug("Position du pointeur %d %d, mouseWin window %d %d, width height %d %d \n",x, y, mouseWin->x, mouseWin->y, mouseWin->width, mouseWin->height);

    // it's time to send mouse Event
    EventPointerMoved pointerMoved(mouseWin->getId(), x, y, state->x, state->y);
    mouseWin->deliverDeviceEvent(&pointerMoved, sizeof(pointerMoved));
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
    int relX = state->x - mouseWin->x;
    int relY = state->y - mouseWin->y;

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
      if (currentChild->mapped && currentChild->contains(this->mouseX, this->mouseY)) {
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

void Mouse::restorePointerBackground() {
  Screen *screen = Screen::getInstance();

  // Restore the previous pointer background
  if (this->pointerBackuped) {
    for (int pBackY = 0; pBackY < PRON_MOUSE_POINTER_HEIGHT; pBackY++) {
      for (int pBackX = 0; pBackX < PRON_MOUSE_POINTER_WIDTH; pBackX++) {
        // The pixel to restore is in  ((pointerBackupX plus old Y position in the screen) times screenWidth plus old X position in the screen ) time bytesPerPixel
        int screenPos = ((pBackY + this->pointerBackupY) * screen->width + pBackX + this->pointerBackupX) * screen->bytesPerPixel;
        int pointerBackupPos = (pBackY * PRON_MOUSE_POINTER_WIDTH + pBackX) * screen->bytesPerPixel;
        //debug("screen [%d] pointerBackup[%d]\n", screenPos, pointerBackupPos);
        char * destination = screen->videoBuffer + screenPos;
        char * source = this->pointerBackup + pointerBackupPos;
        memcpy(destination, source, screen->bytesPerPixel); 
      }
    }
  }
}

void Mouse::drawPointer() {
  Screen *screen = Screen::getInstance();

  // Save the new pointer background
  this->pointerBackupX = this->mouseX;
  this->pointerBackupY = this->mouseY;

  for (int pBackY = 0; pBackY < PRON_MOUSE_POINTER_HEIGHT; pBackY++) {
    for (int pBackX = 0; pBackX < PRON_MOUSE_POINTER_WIDTH; pBackX++) {
      // The pixel to restore is in  ((pointerBackupX plus old Y position in the screen) times screenWidth plus old X position in the screen ) time bytesPerPixel
      int screenPos = ((pBackY + this->mouseY) * screen->width + pBackX + this->mouseX) * screen->bytesPerPixel;
      int pointerBackupPos = (pBackY * PRON_MOUSE_POINTER_WIDTH + pBackX) * screen->bytesPerPixel;
      //debug("screen [%d] pointerBackup[%d]\n", screenPos, pointerBackupPos);
      char * destination = this->pointerBackup + pointerBackupPos;
      char * source = screen->videoBuffer + screenPos;
      memcpy(destination, source, screen->bytesPerPixel); 
    }
  }
  this->pointerBackuped = true;

  // Delete the obsolete clipZone
  screen->setClipWin(NULL);
  
  // We can draw the new pointer
  screen->drawLine(this->mouseX,this->mouseY, this->mouseX, this->mouseY + PRON_MOUSE_POINTER_HEIGHT - 1);
  screen->drawLine(this->mouseX,this->mouseY, this->mouseX + PRON_MOUSE_POINTER_WIDTH - 1, this->mouseY);
}

int Mouse::getPointerBackupX(){
  return this->pointerBackupX;
}

void Mouse::setPointerBackupX(int pointerBackupX){
  this->pointerBackupX = pointerBackupX;
}

int Mouse::getPointerBackupY(){
  return this->pointerBackupY;
}

void Mouse::setPointerBackupY(int pointerBackupY){
  this->pointerBackupY = pointerBackupY;
}

char* Mouse::getPointerBackup(){
  return this->pointerBackup;
}

Mouse* Mouse::getInstance(){
  if (Mouse::instance == NULL) {
    Mouse::instance = new Mouse();
  }
  return Mouse::instance;
}

int Mouse::getMouseX() {
  return this->mouseX;
}

void Mouse::setMouseX(int mouseX) {
  this->mouseX = this->mouseX;
}

int Mouse::getMouseY() {
  return this->mouseY;
}

void Mouse::setMouseY(int mouseY) {
  this->mouseY = this->mouseY;
}

void Mouse::setMouseB1(bool mouseB1) {
  this->mouseB1 = mouseB1;
}

bool Mouse::getMouseB1() {
  return this->mouseB1;
}

void Mouse::setMouseB2(bool mouseB2) {
  this->mouseB2 = mouseB2;
}

bool Mouse::getMouseB2() {
  return this->mouseB2;
}

void Mouse::setMouseB3(bool mouseB3) {
  this->mouseB3 = mouseB3;
}

bool Mouse::getMouseB3() {
  return this->mouseB3;
}

void Mouse::setMouseB4(bool mouseB4) {
  this->mouseB4 = mouseB4;
}

bool Mouse::getMouseB4() {
  return this->mouseB4;
}

void Mouse::setMouseB5(bool mouseB5) {
  this->mouseB5 = mouseB5;
}

bool Mouse::getMouseB5() {
  return this->mouseB5;
}

void Mouse::setMouseB6(bool mouseB6) {
  this->mouseB6 = mouseB6;
}

bool Mouse::getMouseB6() {
  return this->mouseB6;
}
