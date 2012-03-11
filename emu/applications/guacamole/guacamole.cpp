#include <iostream>
#include <time.h>
#include <clibtacos>
#include <cstdio>
#include <vector>

#include <pronlib.h>

using namespace std;

int main() {
  srand(time(NULL));
  
  // connection to pron
  Display *display = pronConnect();
  
  // Get the root window informations
  PronWindowAttributes rootWindowAttributes;
  pronGetWindowAttributes(display, display->rootWindow, &rootWindowAttributes);
  
  debug("Root window width : %d, height : %d\n", rootWindowAttributes.width, rootWindowAttributes.height);
  
  // Subscribe to window creation events
  pronSelectInput(display, display->rootWindow, PRON_EVENTMASK(EV_WINDOW_CREATED) | PRON_EVENTMASK(EV_KEY_PRESSED) | PRON_EVENTMASK(EV_KEY_RELEASED));
  PronEvent *e = getPronEvent();

  vector<Window> windows;
  int w_idx = 0;

  while (1) {
    if (!pronNextEvent(display, e)) {
      fprintf(stderr, "pron has closed the connection.\n");
      exit(1);
    }

    switch (e->type) {
      case EV_WINDOW_CREATED : {
        debug("EVENT_WINDOW_CREATED reçu\n");
        EventWindowCreated *windowCreated = (EventWindowCreated*) e;
        windowCreated->attributes.x = rand() % (rootWindowAttributes.width  - windowCreated->attributes.width );
        windowCreated->attributes.y = rand() % (rootWindowAttributes.height - windowCreated->attributes.height);
        pronSetWindowAttributes(display, windowCreated->window, windowCreated->attributes, WIN_ATTR_X | WIN_ATTR_Y);
        if (windowCreated->parent == 0) {
          printf("top level window\n");
          windows.push_back(windowCreated->window);
        }
        break;
      }
      case EV_KEY_PRESSED : {
        EventKeyPressed *keyPressed = (EventKeyPressed*) e;
        printf("Key pressed : %d\n", keyPressed->keysym);
        if (keyPressed->keysym == PRONK_TAB && !windows.empty()) {
          w_idx = (w_idx + 1) % windows.size();
          printf("raise window %d\n", windows[w_idx]);
          pronRaiseWindow(display, windows[w_idx]);
        }
        break;
      }
      case EV_KEY_RELEASED : {
        EventKeyReleased *keyReleased = (EventKeyReleased*) e;
        printf("Key released : %d\n", keyReleased->keysym);
        break;
      }
      default:
        break;
    }
  }
}
