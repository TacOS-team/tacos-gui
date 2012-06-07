#include <vector>
#include <errno.h>
#include "sombrero.h"
#include "proto/events.h"

namespace sombrero {

Application* Application::instance = NULL;

Application::Application() {
  sombrero_ascii_art();
  this->d = pron::pronConnect();
  if (d == NULL) {
    fprintf(stderr, "Sombrero : error while connecting to pron server\n");
    exit(1);
  }
  Application::instance = this;
}

Application::~Application() {
  pron::pronDisconnect(d);
  delete d;
}

Application* Application::getInstance() {
  if (Application::instance == NULL) {
    new Application();
  } 
  return Application::instance;
}

void Application::windowCreated(pron::EventWindowCreated *e __attribute__((unused))) {

}

void Application::windowDestroyed(pron::EventDestroyWindow *e __attribute__((unused))) {

}

void Application::keyPressed(pron::EventKeyPressed *e __attribute__((unused))) {

}

void Application::keyReleased(pron::EventKeyReleased *e __attribute__((unused))) {

}

// Call every handleEvent*** method for the given events
// handleEvent*** can be redefined in the child classes
void Application::sombrerun() {
  pron::PronEvent *e = pron::getPronEvent();

  /*printf("DEBUG éléments map application\n");
  for(map<pron::Window, Widget*>::iterator it = Application::getInstance()->widgets.begin(); it != Application::getInstance()->widgets.end(); ++it) {
    printf("New element %x -> %p \n", it->first, it->second);
  }*/
  while (1) {
    if (!pron::pronNextEvent(this->d, e)) {
      fprintf(stderr, "pron has closed the connection, exiting.\n");
      exit(1);
    }

    if (e->type == pron::EV_WINDOW_CREATED) {
      this->windowCreated((pron::EventWindowCreated*) e);
    } else if (e->type == pron::EV_DESTROY_WINDOW) {
      this->windowDestroyed((pron::EventDestroyWindow*) e);
    } else if (e->type == pron::EV_KEY_PRESSED) {
      this->keyPressed((pron::EventKeyPressed*) e);
    } else if (e->type == pron::EV_KEY_RELEASED) {
      this->keyReleased((pron::EventKeyReleased*) e);
    }

    //printf("event received %d\n", e->window);

    map<pron::Window, Widget*>::iterator it = Application::getInstance()->widgets.find(e->window);
    if(it != Application::getInstance()->widgets.end()) {
      Widget *w = it->second;
      switch (e->type) {
        case pron::EV_WINDOW_CREATED:
          w->handleEventWindowCreated();
          break;
        case pron::EV_EXPOSE:
          w->handleEventExpose();
          break;
        case pron::EV_POINTER_MOVED:
          w->handleEventPointerMoved((pron::EventPointerMoved*) e);
          break;
        case pron::EV_MOUSE_BUTTON:
          w->handleEventMouseButton((pron::EventMouseButton*) e);
          break;
        case pron::EV_KEY_PRESSED:
          w->handleEventKeyPressed((pron::EventKeyPressed*) e);
          break;
        case pron::EV_KEY_RELEASED:
          w->handleEventKeyReleased((pron::EventKeyReleased*) e);
          break;
        case pron::EV_DESTROY_WINDOW:
          w->handleEventDestroyWindow();
          break;
        case pron::EV_RESIZE_WINDOW: {
          pron::EventResizeWindow *resizeEvent = (pron::EventResizeWindow*) e;
          w->handleEventResizeWindow(resizeEvent->width, resizeEvent->height);
          break;
        }
        default:
          break; 
      }
    }
  }
}

} // namespace sombrero
