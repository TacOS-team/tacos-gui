#include <vector>
#include <errno.h>
#include "sombrero.h"

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
    while (1) {
      if (!pron::pronNextEvent(this->d, e)) {
	if (errno == EINTR) continue;
	fprintf(stderr, "lol pron has closed the connection.\n");
	exit(1);
      }

      Widget *w = Application::getInstance()->widgets[e->window];
      switch (e->type) {
	case pron::EV_WINDOW_CREATED:
	  w->handleEventWindowCreated();
	  break;
	case pron::EV_EXPOSE:
	  w->handleEventExpose();
	  break;
	case pron::EV_POINTER_MOVED:
	  w->handleEventPointerMoved();
	  break;
	case pron::EV_MOUSE_BUTTON:
	  w->handleEventMouseButton(e);
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
	case pron::EV_RESIZE_WINDOW:
	  w->handleEventResizeWindow();
	  break;
	default:
	  break; 
      }
    }
  }

} // namespace sombrero
