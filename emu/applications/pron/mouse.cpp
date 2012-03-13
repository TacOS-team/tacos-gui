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
  this->pointerBackupX = 0;
  this->pointerBackupY = 0;
  Screen *screen = Screen::getInstance();
  this->pointerBackup = (char *)malloc(PRON_MOUSE_POINTER_WIDTH * PRON_MOUSE_POINTER_HEIGHT * screen->bytesPerPixel);
  memset(this->pointerBackup, 0, PRON_MOUSE_POINTER_WIDTH * PRON_MOUSE_POINTER_HEIGHT * screen->bytesPerPixel);
  this->pointerBackuped = false;
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

    // We have to recompute the mouseWin
    this->updateMouseWin();
    Window *mouseWin = screen->getMouseWin();

    //computing relative coordinates 
    int x = state.x - mouseWin->x;
    int y = state.y - mouseWin->y;
    
    // send the event to the current mouseWin
    //debug("Position du pointeur %d %d, mouseWin window %d %d, width height %d %d \n",x, y, mouseWin->x, mouseWin->y, mouseWin->width, mouseWin->height);

    // it's time to send mouse Event
    EventPointerMoved pointerMoved(mouseWin->id, x, y, state.x, state.y);
    mouseWin->deliverDeviceEvent(&pointerMoved, sizeof(pointerMoved));
  }

  // Draws the mouse pointer
  this->drawPointer();
}

void Mouse::updateMouseWin(){
  Screen *screen = Screen::getInstance();
  int mouseX = screen->getMouseX();
  int mouseY = screen->getMouseY();
  // We have to explorate window tree from the  screen to his children
  // and check if the pointer is inside them : We can stop when we have found one window
  // because it is necessarily the one which is drawn
  Window *currentWin = screen->getRoot();
  // We stop when currentWin has no children 
  while(currentWin->lastChild != NULL){
    // we have to run throug the childs from the right to the left
    // and take the first (the one with the bigger Z)
    Window *currentChild = currentWin->lastChild;
    while(currentChild != NULL){
      if(currentChild->contains(mouseX, mouseY)){
        currentWin = currentChild;
        break;// We have found the first containing child we have to stop
      } 
      currentChild = currentChild->prevSibling;
    }
    if(currentChild == NULL){
      break;// We have no child which contains the pointer we can stop
    }
  }

  // MouseWin is computed we can store it in the screen to increzse performances
  screen->setMouseWin(currentWin);
}

void Mouse::drawPointer(){
  Screen *screen = Screen::getInstance();
  // Restore the previous pointer background
  if(this->pointerBackuped){
    for(int pBackY = 0 ; pBackY < PRON_MOUSE_POINTER_HEIGHT ; pBackY++){
      for(int pBackX = 0 ; pBackX < PRON_MOUSE_POINTER_WIDTH ; pBackX++){
        // The pixel to restore is in  ((pointerBackupX plus old Y position in the screen) times screenWidth plus old X position in the screen ) time bytesPerPixel
        int screenPos = ((pBackY + this->pointerBackupY) * screen->width + pBackX + this->pointerBackupX) * screen->bytesPerPixel;
        int pointerBackupPos = (pBackY * screen->width + pBackX) * screen->bytesPerPixel;
        //debug("screen [%d] pointerBackup[%d]\n", screenPos, pointerBackupPos);
        char * destination = screen->videoBuffer + screenPos;
        char * source = this->pointerBackup + pointerBackupPos;
        memcpy(destination, source, screen->bytesPerPixel); 
      }
    }
  }

  // Save the new pointer background
  this->pointerBackupX = screen->getMouseX();
  this->pointerBackupY = screen->getMouseY();

  for(int pBackY = 0 ; pBackY < PRON_MOUSE_POINTER_HEIGHT ; pBackY++){
    for(int pBackX = 0 ; pBackX < PRON_MOUSE_POINTER_WIDTH ; pBackX++){
      // The pixel to restore is in  ((pointerBackupX plus old Y position in the screen) times screenWidth plus old X position in the screen ) time bytesPerPixel
      int screenPos = ((pBackY + screen->getMouseY()) * screen->width + pBackX + screen->getMouseX()) * screen->bytesPerPixel;
      int pointerBackupPos = (pBackY * screen->width + pBackX) * screen->bytesPerPixel;
      //debug("screen [%d] pointerBackup[%d]\n", screenPos, pointerBackupPos);
      char * destination = this->pointerBackup + pointerBackupPos;
      char * source = screen->videoBuffer + screenPos;
      memcpy(destination, source, screen->bytesPerPixel); 
    }
  }
  this->pointerBackuped = true;

  // Delete the obsolete clipZone
  screen->clipWin = NULL;
  delete screen->clipZone;
  screen->clipZone = NULL;
  // We can draw the new pointer
  screen->drawLine(screen->getMouseX(),screen->getMouseY(), screen->getMouseX(), screen->getMouseY() + PRON_MOUSE_POINTER_HEIGHT - 1);//XXX : -1 : bug de drawline
  screen->drawLine(screen->getMouseX(),screen->getMouseY(), screen->getMouseX() + PRON_MOUSE_POINTER_WIDTH - 1, screen->getMouseY());//XXX : -1 : bug de drawline
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
