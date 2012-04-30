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

void Application::sombrerun() {
  pron::PronEvent *e = pron::getPronEvent();
  while (1) {
    if (!pron::pronNextEvent(this->d, e)) {
      if (errno == EINTR) continue;
      fprintf(stderr, "lol pron has closed the connection.\n");
      exit(1);
    }

    // Dispatch the event to the target widget
    Widget *w = Application::getInstance()->widgets[e->window];
    w->handleEvent(e);
  }
}

} // namespace sombrero
