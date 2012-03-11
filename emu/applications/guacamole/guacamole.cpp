#include <iostream>
#include <time.h>
#include <clibtacos>
#include <cstdio>
#include <vector>
#include <algorithm>


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
        if (std::find(windows.begin(), windows.end(), windowCreated->window) == windows.end()) {
          //windowCreated->attributes.x = rand() % (rootWindowAttributes.width  - windowCreated->attributes.width );
          //windowCreated->attributes.y = rand() % (rootWindowAttributes.height - windowCreated->attributes.height);
          //pronSetWindowAttributes(display, windowCreated->window, windowCreated->attributes, WIN_ATTR_X | WIN_ATTR_Y);
          Window parentWindowId = pronCreateWindow(display, display->rootWindow,
                            max(windowCreated->attributes.x - 15, 0), max(windowCreated->attributes.y - 15, 0),
                            windowCreated->attributes.width + 30, windowCreated->attributes.height + 30);
          PronWindowAttributes newAttr;
          COLOR(newAttr.bgColor, 24).r = 255;
          COLOR(newAttr.bgColor, 24).g = 0;
          COLOR(newAttr.bgColor, 24).b = 0;
          pronSetWindowAttributes(display, parentWindowId, newAttr, WIN_ATTR_BG_COLOR);
          pronClearWindow(display, parentWindowId);
          pronReparentWindow(display, windowCreated->window, parentWindowId);
          if (windowCreated->parent == 0) {
            printf("top level window\n");
            windows.push_back(windowCreated->window);
            windows.push_back(parentWindowId);
          }
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
