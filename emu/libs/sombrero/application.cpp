#include <vector>
#include "application.h"
#include "pronlib.h"

namespace sombrero {

Application* Application::instance = NULL;

Application::Application() 
  : windows() {
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

} // namespace sombrero
