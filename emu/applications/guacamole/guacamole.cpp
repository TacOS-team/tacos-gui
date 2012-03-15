#include <iostream>
#include <time.h>
#include <clibtacos>
#include <cstdio>
#include <vector>
#include <algorithm>


#include <pronlib.h>
#include <gwindows_manager.h>

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
  pronSelectInput(display, display->rootWindow,
    PRON_EVENTMASK(EV_WINDOW_CREATED) | PRON_EVENTMASK(EV_KEY_PRESSED)
    | PRON_EVENTMASK(EV_KEY_RELEASED) | PRON_EVENTMASK(EV_POINTER_MOVED) );
  PronEvent *e = getPronEvent();

  GWindowsManager windowsManager;
  

  //int w_idx = 0;

  // TODO
  // Très moche mais très provisoirement ça marche :D
  unsigned int windowIdLeftButtonPressed = 0;
  int mouseLastXPosition = 0;
  int mouseLastYPosition = 0;

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

        // we don't want to add a window that is already known
        if (windowsManager.getGWindow(windowCreated->window) != NULL) {
          winAlreadyExists = true;
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

            // Fond rouge à la fenêtre de déco provisoire
            PronWindowAttributes newAttr;
            COLOR(newAttr.bgColor, 24).r = 255;
            COLOR(newAttr.bgColor, 24).g = 0;
            COLOR(newAttr.bgColor, 24).b = 0;
            pronSetWindowAttributes(display, parentWindowId, newAttr, WIN_ATTR_BG_COLOR);
            
            pronMapWindow(display, parentWindowId);

            // Abonnement aux évènements souris de la fenêtre de décoration
            pronSelectInput(display, parentWindowId,
                  PRON_EVENTMASK(EV_MOUSE_BUTTON));
            
            pronReparentWindow(display, windowCreated->window, parentWindowId);
            
            GWindow *gw = new GWindow(windowCreated->window, parentWindowId);
            windowsManager.addGWindow(gw);
            // registering for any EV_DESTROY_WINDOW event
            pronSelectInput(display, windowCreated->window, PRON_EVENTMASK(EV_DESTROY_WINDOW));
          }
        }
        break;
      }
      case EV_KEY_PRESSED : {
        EventKeyPressed *keyPressed = (EventKeyPressed*) e;
        printf("Key pressed : %d\n", keyPressed->keysym);
        // TODO a recoder avec le windowsManager
        /*if (keyPressed->keysym == PRONK_TAB && !windowsManager.empty()) {
          w_idx = (w_idx + 1) % windows.size();
          printf("raise window %d\n", windows[w_idx].parent);
          pronRaiseWindow(display, windows[w_idx].parent);
        }*/
        break;
      }
      case EV_KEY_RELEASED : {
        EventKeyReleased *keyReleased = (EventKeyReleased*) e;
        printf("Key released : %d\n", keyReleased->keysym);
        break;
      }
      case EV_DESTROY_WINDOW : {
        EventDestroyWindow *destroyWindowEvent = (EventDestroyWindow*) e;
        printf("DestroyWindow event received for %d\n", destroyWindowEvent->window);

        // Sending destroy request for the parent window
        printf("état de la liste des windows\n");
        windowsManager.destroy(destroyWindowEvent->window);
        break;
      }
      case EV_MOUSE_BUTTON : {
        EventMouseButton *mouseButtonEvent = (EventMouseButton*) e;
        if (mouseButtonEvent->b1) {
          // Si on a bien sélectionné la décoration
          GWindow *gwin = windowsManager.getGWindow(mouseButtonEvent->window);
          if (mouseButtonEvent->window == gwin->parent) {
            windowIdLeftButtonPressed = mouseButtonEvent->window;
          }
          pronRaiseWindow(display, gwin->parent);
        } else {
          windowIdLeftButtonPressed = 0;
        }
        break;
      }
      case EV_POINTER_MOVED : {
        EventPointerMoved *mousePointerEvent = (EventPointerMoved*) e;
        if (windowIdLeftButtonPressed) {
          pronMoveWindow(display, windowIdLeftButtonPressed,
            mousePointerEvent->xRoot - mouseLastXPosition,
            mousePointerEvent->yRoot - mouseLastYPosition);
        }
        mouseLastXPosition = mousePointerEvent->xRoot;
        mouseLastYPosition = mousePointerEvent->yRoot;
        break;
      }
      default:
        break;
    }
  }
}
