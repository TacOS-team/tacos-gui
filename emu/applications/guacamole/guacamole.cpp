#include <iostream>
#include <time.h>
#include <clibtacos>

#include <pronlib.h>


int main() {
  srand ( time(NULL) );
  
  // connection to pron
  Display * display = pronConnect();
  
  // Get the root window informations
  PronWindowAttributes rootWindowAttributes;
  pronGetWindowAttributes(display, display->rootWindow, &rootWindowAttributes);
  
  debug("width : %d, height : %d\n", rootWindowAttributes.width, rootWindowAttributes.height);
  
  // Subscribe to window creation events
  pronSelectInput(display, display->rootWindow, CREATE_NOTIFY);
  PronEvent * e = getPronEvent();
  while(1) {
    pronNextEvent(display, e);
    switch (e->type) {
      case(EVENT_WINDOW_CREATED) : {
        debug("EVENT_WINDOW_CREATED reçu\n");
        EventWindowCreated * eventWindowCreated = (EventWindowCreated *) e;
        eventWindowCreated->attributes.x = rand() % (rootWindowAttributes.width  - eventWindowCreated->attributes.width );
        eventWindowCreated->attributes.y = rand() % (rootWindowAttributes.height - eventWindowCreated->attributes.height);
        pronSetWindowAttributes(display, eventWindowCreated->window, eventWindowCreated->attributes, WIN_ATTR_X | WIN_ATTR_Y);
        break;
      }
      default:
        break;
    }
  }
}
