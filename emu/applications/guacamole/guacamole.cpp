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
  GWindowsManager windowsManager(display->rootWindow);
  pronGetWindowAttributes(display, display->rootWindow, &windowsManager.getRootWindowAttributes());
  
  debug("Root window width : %d, height : %d\n", windowsManager.getRootWindowAttributes().width,
          windowsManager.getRootWindowAttributes().height);
  
  // Subscribe to root window events
  int currentRootEventMask =  PRON_EVENTMASK(EV_WINDOW_CREATED)
                            | PRON_EVENTMASK(EV_KEY_PRESSED)
                            | PRON_EVENTMASK(EV_KEY_RELEASED);
  pronSelectInput(display, display->rootWindow, currentRootEventMask);
  PronEvent *e = getPronEvent();
  

  // TODO
  // Très moche mais très provisoirement ça marche :D
  unsigned int windowIdLeftButtonPressed = 0;
  int mouseLastXPosition = -1;
  int mouseLastYPosition = -1;

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
            Window parentWindowId = pronCreateWindow(display, display->rootWindow,
                windowCreated->attributes.x - 15, windowCreated->attributes.y - 15,
                windowCreated->attributes.width + 30, windowCreated->attributes.height + 30);
            
            pronMapWindow(display, parentWindowId);

            // Abonnement aux évènements souris de la fenêtre de décoration
            pronSelectInput(display, parentWindowId,
                  PRON_EVENTMASK(EV_MOUSE_BUTTON));
            
            pronReparentWindow(display, windowCreated->window, parentWindowId);
            
            GWindow *gw = new GWindow(windowCreated->window, parentWindowId);
            gw->attributes = windowCreated->attributes;
            // registering for any EV_DESTROY_WINDOW event
            pronSelectInput(display, windowCreated->window,
                PRON_EVENTMASK(EV_DESTROY_WINDOW) || PRON_EVENTMASK(EV_MOUSE_BUTTON));
            /*printf("avant get %d %d\n", gw->parentAttributes.width, gw->parentAttributes.height);
            pronGetWindowAttributes(display, parentWindowId, &(gw->parentAttributes));
            printf("apres get %d %d\n", gw->parentAttributes.width, gw->parentAttributes.height);*/
            // Calcul provisoire en attendant que le getattribute marche correctement
            gw->parentAttributes.width = windowCreated->attributes.width + 30;
            gw->parentAttributes.height = windowCreated->attributes.height + 30;

            // Fond rouge à la fenêtre de déco provisoire
            COLOR(gw->parentAttributes.bgColor, 24).r = 255;
            COLOR(gw->parentAttributes.bgColor, 24).g = 0;
            COLOR(gw->parentAttributes.bgColor, 24).b = 0;
            pronSetWindowAttributes(display, parentWindowId, gw->parentAttributes, WIN_ATTR_BG_COLOR);

            windowsManager.initWindowPosition(gw);

            printf("nouvelle position %d %d\n", gw->parentAttributes.x, gw->parentAttributes.y);
            pronMoveWindow(display, gw->parent, gw->parentAttributes.x, gw->parentAttributes.y);

            windowsManager.addGWindow(gw);
          }
        }
        break;
      }
      case EV_KEY_PRESSED : {
        EventKeyPressed *keyPressed = (EventKeyPressed*) e;
        printf("Key pressed : %d\n", keyPressed->keysym);
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
          // If the window is the decoration window
          GWindow *gwin = windowsManager.getGWindow(mouseButtonEvent->window);
          if (mouseButtonEvent->window == gwin->parent) {
            windowIdLeftButtonPressed = mouseButtonEvent->window;
          }
          // Puts the window on foreground
          pronRaiseWindow(display, gwin->parent);
          // Subscribe to pointer moved and mouse button of the root window
          //   to avoid problems if it moves too fast
          currentRootEventMask |= PRON_EVENTMASK(EV_POINTER_MOVED) | PRON_EVENTMASK(EV_MOUSE_BUTTON);
          // Ask to reset the last mouse position
          mouseLastXPosition = -1;
        } else {
          windowIdLeftButtonPressed = 0;
          // Unsubscribe of events of the root window to avoid useless events
          currentRootEventMask &= ~PRON_EVENTMASK(EV_MOUSE_BUTTON);
          currentRootEventMask &= ~PRON_EVENTMASK(EV_POINTER_MOVED);
        }
        pronSelectInput(display, display->rootWindow, currentRootEventMask);
        break;
      }
      case EV_POINTER_MOVED : {
        EventPointerMoved *mousePointerEvent = (EventPointerMoved*) e;
        if (windowIdLeftButtonPressed && mouseLastXPosition != -1) {
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
