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
      windowLeftButtonPressed->move(xMove, yMove);
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
  
  GWindowsManager::init(display->rootWindow);
  // Get the root window informations
  pronGetWindowAttributes(display, display->rootWindow, &GWindowsManager::getInstance()->getRootWindowAttributes());
  
  debug("Root window width : %d, height : %d\n", GWindowsManager::getInstance()->getRootWindowAttributes().width,
    GWindowsManager::getInstance()->getRootWindowAttributes().height);
  
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
        if (GWindowsManager::getInstance()->getGWindow(windowCreated->window) != NULL) {
          winAlreadyExists = true;
        }
        if (!winAlreadyExists && windowCreated->parent == 0) {
          printf("top level window\n");
          
          new GWindow(windowCreated->window, windowCreated->attributes, true, display);
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
        GWindow *pgwin = GWindowsManager::getInstance()->getGWindow(destroyWindowEvent->window);
        if (pgwin) {
          GWindowsManager::getInstance()->destroy(destroyWindowEvent->window);
        }
        break;
      }
      case EV_MOUSE_BUTTON : {
        EventMouseButton *mouseButtonEvent = (EventMouseButton*) e;
        printf("mouse button event. ID : %d\n", mouseButtonEvent->window);
        GWindow *gwin = GWindowsManager::getInstance()->getGWindow(mouseButtonEvent->window);
        if (gwin && mouseButtonEvent->b1) {
          // If the window is the decoration window
          if (mouseButtonEvent->window == gwin->closeButton) {
            GWindowsManager::getInstance()->destroy(gwin->parent);
          } else if (mouseButtonEvent->window == gwin->maximiseButton) {
            gwin->maximise();
          } else {
            if (mouseButtonEvent->window == gwin->resizeButton) {
              windowResizeButtonPressed = gwin;
            }
            else if (mouseButtonEvent->window == gwin->parent) {
              windowLeftButtonPressed = gwin;
            }
            // Subscribe to pointer moved and mouse button of the root window
            //   to avoid problems if it moves too fast
            currentRootEventMask |= PRON_EVENTMASK(EV_POINTER_MOVED) | PRON_EVENTMASK(EV_MOUSE_BUTTON);
            // Ask to reset the last mouse position
            mouseActualXPosition = -1;
          }
          // Puts the window on foreground
          gwin->raise();
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
        GWindow *gwin = GWindowsManager::getInstance()->getGWindow(mouseExposeEvent->window);
        if (gwin) {
          gwin->decorate();
        }

      }
      default:
      break;
    }
  }
}
