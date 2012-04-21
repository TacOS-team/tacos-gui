#include <vector>
#include "application.h"
#include "pronlib.h"
#include "window.h"

namespace sombrero {

  Application* Application::instance = NULL;

  Application::Application() 
    : windows() {
    
  }

  Application::~Application() {
    
  }

  Application* Application::getInstance() {
    if (Application::instance == NULL) {
      Application::instance = new Application();
    } 
    return Application::instance;
  }

  std::vector<Window*> Application::getWindows() {
    return windows;
  }

  void Application::addWindow(Window &window) {
    windows.push_back(&window);
  }

  void Application::removeWindow(Window &window) {
    vector<Window*>::iterator it;
    for (it = this->windows.begin(); it < this->windows.end(); it++) {
      if (*it == &window) {
	windows.erase(it, it);
      }
    }    
  }

}
