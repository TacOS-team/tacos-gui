#include <iostream>
#include <time.h>
#include <clibtacos>
#include <cstdio>

#include <pronlib.h>

int main() {
  srand ( time(NULL) );
  
  // connection to pron
  Display * display = pronConnect();
  
  // Get the root window informations
  PronWindowAttributes rootWindowAttributes;
  pronGetWindowAttributes(display, display->rootWindow, &rootWindowAttributes);
  
  debug("Root window width : %d, height : %d\n", rootWindowAttributes.width, rootWindowAttributes.height);
  
  // Subscribe to window creation events
  pronSelectInput(display, display->rootWindow, PRON_EVENTMASK(EV_WINDOW_CREATED));
  PronEvent * e = getPronEvent();
  while (1) {
    if (!pronNextEvent(display, e)) {
      fprintf(stderr, "pron has closed the connection.\n");
      exit(1);
    }
    switch (e->type) {
      case EV_WINDOW_CREATED : {
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
