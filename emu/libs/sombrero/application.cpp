#include <vector>
#include <errno.h>
#include "sombrero.h"
#include "proto/events.h"

namespace sombrero {

  Application* Application::instance = NULL;

  void Application::init() {
    sombrero_ascii_art();
  }

  Application::Application() {
    this->d = pron::pronConnect();
    if (d == NULL) {
      fprintf(stderr, "Sombrero : error while connecting to pron server\n");
      exit(1);
    }
  }

  Application::~Application() {
    pron::pronDisconnect(d);
    delete d;
  }

  Application* Application::getInstance() {
    if (Application::instance == NULL) {
      Application::instance = new Application();
    } 
    return Application::instance;
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
        if (errno == EINTR) {
          continue;
        }
        fprintf(stderr, "lol pron has closed the connection.\n");
        exit(1);
      }

      //printf("event received\n");

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
            w->handleEventMouseButton((pron::EventMouseButton*)e);
            break;
          case pron::EV_KEY_PRESSED:
            w->handleEventKeyPressed(e);
            break;
          case pron::EV_KEY_RELEASED:
            w->handleEventKeyReleased();
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
