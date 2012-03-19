#include <iostream>
#include <time.h>
#include <clibtacos>
#include <cstdio>
#include <vector>
#include <algorithm>
#include <signal.h>
#include <sys/time.h>


#include <pronlib.h>
#include <gwindows_manager.h>

using namespace std;


GWindow *windowLeftButtonPressed   = NULL;
GWindow *windowResizeButtonPressed = NULL;
int mouseLastXPosition = -1;
int mouseLastYPosition = -1;
int mouseActualXPosition = -1;
int mouseActualYPosition = -1;
bool isTimerActive = false;

void derout(int signum)
{
  if(SIGALRM == signum)
  {
    isTimerActive = false;
    struct itimerval timer;
    // We stop the timer
    timer.it_value.tv_sec  = 0;
    timer.it_value.tv_usec = 0;
    timer.it_interval = timer.it_value;
    setitimer(ITIMER_REAL, &timer, (struct itimerval*)NULL);

    int xMove = mouseLastXPosition - mouseActualXPosition;
    int yMove = mouseLastYPosition - mouseActualYPosition;

    if (windowLeftButtonPressed) {
      pronMoveWindow(display, windowLeftButtonPressed->parent, xMove, yMove);
      windowLeftButtonPressed->parentAttributes.x += xMove;
      windowLeftButtonPressed->parentAttributes.y += yMove;
      windowLeftButtonPressed->attributes.x       += xMove;
      windowLeftButtonPressed->attributes.y       += yMove;
    } else if (windowResizeButtonPressed) {
      windowResizeButtonPressed->resize(
        windowResizeButtonPressed->parentAttributes.width  + xMove,
        windowResizeButtonPressed->parentAttributes.height + yMove);
    }
    mouseActualXPosition = mouseLastXPosition;
    mouseActualYPosition = mouseLastYPosition;
  }
}

void activateTimer(unsigned int usec) {
  struct itimerval timer;

  if (!isTimerActive) {
    timer.it_value.tv_sec  = 0;
    timer.it_value.tv_usec = usec;
    timer.it_interval = timer.it_value;
    setitimer(ITIMER_REAL, &timer, (struct itimerval*)NULL);
    isTimerActive = true;
  }
}


int main() {
  srand(time(NULL));
  signal(SIGALRM, &derout);
  
  // connection to pron
  display = pronConnect();
  
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
        if (!winAlreadyExists && windowCreated->parent == 0) {
          printf("top level window\n");
          
          GWindow *gw = new GWindow(windowCreated->window, windowCreated->attributes, true, display);

          windowsManager.initWindowPosition(gw);
          printf("nouvelle position %d %d\n", gw->parentAttributes.x, gw->parentAttributes.y);
          pronMoveWindow(display, gw->parent, gw->parentAttributes.x, gw->parentAttributes.y);

          windowsManager.addGWindow(gw);
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
        GWindow *pgwin = windowsManager.getGWindow(destroyWindowEvent->window);
        if (pgwin) {
          if (pgwin->parent != destroyWindowEvent->window) {
            pronDestroyWindow(display,pgwin->parent);
          }
          windowsManager.destroy(destroyWindowEvent->window);
        }
        break;
      }
      case EV_MOUSE_BUTTON : {
        EventMouseButton *mouseButtonEvent = (EventMouseButton*) e;
        printf("mouse button event. ID : %d\n", mouseButtonEvent->window);
        if (mouseButtonEvent->b1) {
          // If the window is the decoration window
          GWindow *gwin = windowsManager.getGWindow(mouseButtonEvent->window);
          if (mouseButtonEvent->window == gwin->closeButton) {
            pronDestroyWindow(display,gwin->parent);
            windowsManager.destroy(gwin->parent);
          } else {
            if (mouseButtonEvent->window == gwin->resizeButton) {
              windowResizeButtonPressed = gwin;
            }
            else if (mouseButtonEvent->window == gwin->parent) {
              windowLeftButtonPressed = gwin;
            }
            // Puts the window on foreground
            pronRaiseWindow(display, gwin->parent);
            // Subscribe to pointer moved and mouse button of the root window
            //   to avoid problems if it moves too fast
            currentRootEventMask |= PRON_EVENTMASK(EV_POINTER_MOVED) | PRON_EVENTMASK(EV_MOUSE_BUTTON);
            // Ask to reset the last mouse position
            mouseActualXPosition = -1;
          }
        } else {
          windowLeftButtonPressed   = NULL;
          windowResizeButtonPressed = NULL;
          // Unsubscribe of events of the root window to avoid useless events
          currentRootEventMask &= ~PRON_EVENTMASK(EV_MOUSE_BUTTON);
          currentRootEventMask &= ~PRON_EVENTMASK(EV_POINTER_MOVED);
        }
        pronSelectInput(display, display->rootWindow, currentRootEventMask);
        break;
      }
      case EV_POINTER_MOVED : {
        EventPointerMoved *mousePointerEvent = (EventPointerMoved*) e;
        if (mouseActualXPosition == -1) {
          mouseActualXPosition = mousePointerEvent->xRoot;
          mouseActualYPosition = mousePointerEvent->yRoot;
        } else {
          activateTimer(60000);
        }
        mouseLastXPosition = mousePointerEvent->xRoot;
        mouseLastYPosition = mousePointerEvent->yRoot;
        break;
      }
      case EV_EXPOSE : {
        EventExpose *mouseExposeEvent = (EventExpose*) e;
        GWindow *gwin = windowsManager.getGWindow(mouseExposeEvent->window);
        gwin->decorate();

      }
      default:
      break;
    }
  }
}
