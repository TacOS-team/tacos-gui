#include <iostream>
#include <time.h>
#include <clibtacos>
#include <cstdio>
#include <vector>
#include <algorithm>


#include <pronlib.h>

using namespace std;

struct windowInfo {
  Window w;
  Window parent;
};

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

  vector<struct windowInfo> windows;
  

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
        bool winAlreadyExists = false;

        for (unsigned int i = 0; i< windows.size();i++) {
          if (windows[i].w == windowCreated->window || windows[i].parent == windowCreated->window) {
            winAlreadyExists = true;
            break;
          }
        }
        if (!winAlreadyExists) {
          if (windowCreated->parent == 0) {
            printf("top level window\n");
            /*
            windowCreated->attributes.x = rand() % (rootWindowAttributes.width  - windowCreated->attributes.width );
            windowCreated->attributes.y = rand() % (rootWindowAttributes.height - windowCreated->attributes.height);
            pronSetWindowAttributes(display, windowCreated->window, windowCreated->attributes, WIN_ATTR_X | WIN_ATTR_Y);
            */
            Window parentWindowId = pronCreateWindow(display, display->rootWindow,
                windowCreated->attributes.x - 15, windowCreated->attributes.y - 15,
                windowCreated->attributes.width + 30, windowCreated->attributes.height + 30);

            PronWindowAttributes newAttr;
            COLOR(newAttr.bgColor, 24).r = 255;
            COLOR(newAttr.bgColor, 24).g = 0;
            COLOR(newAttr.bgColor, 24).b = 0;
            pronSetWindowAttributes(display, parentWindowId, newAttr, WIN_ATTR_BG_COLOR);
            
            pronMapWindow(display, parentWindowId);
            
            pronReparentWindow(display, windowCreated->window, parentWindowId);
            
            struct windowInfo winInfo = {windowCreated->window, parentWindowId};
            windows.push_back(winInfo);

            pronSelectInput(display, windowCreated->window, PRON_EVENTMASK(EV_DESTROY_WINDOW));
          }
        }
        break;
      }
      case EV_KEY_PRESSED : {
        EventKeyPressed *keyPressed = (EventKeyPressed*) e;
        printf("Key pressed : %d\n", keyPressed->keysym);
        if (keyPressed->keysym == PRONK_TAB && !windows.empty()) {
          w_idx = (w_idx + 1) % windows.size();
          printf("raise window %d\n", windows[w_idx].w);
          pronRaiseWindow(display, windows[w_idx].w);
        }
        break;
      }
      case EV_KEY_RELEASED : {
        EventKeyReleased *keyReleased = (EventKeyReleased*) e;
        printf("Key released : %d\n", keyReleased->keysym);
        break;
      }
      case EV_DESTROY_WINDOW : {
        EventDestroyWindow * destroyWindowEvent = (EventDestroyWindow*) e;
        printf("DestroyWindow received for %d\n", destroyWindowEvent->window);

        // Sending destroy request for the parent window
        printf("état de la liste des windows\n");
        for (unsigned int index = 0; index < windows.size(); index++) {
          printf("(%d %d)",windows[index].w, windows[index].parent);
          printf("\n");
        }
        for (unsigned int index = 0; index < windows.size(); index++) {
          if (windows[index].w == destroyWindowEvent->window) {
            printf("send destroy request for id %d\n", windows[index].parent);
            pronDestroyWindow(display,windows[index].parent);
            windows.erase(windows.begin() + index);
            break;
          }
        }
        break;
      }
      default:
      break;
    }
  }
}
