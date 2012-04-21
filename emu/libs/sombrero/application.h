#ifndef __APPLICATION_H__
#define __APPLICATION_H__

#include <vector>
#include "pronlib.h"
#include "window.h"

namespace sombrero {

  class Window;
  
  class Application {

  private:
    static Application *instance;
    vector<Window*> windows;
    Application();
    ~Application();

  public:
    static Application* getInstance();
    void run();
    std::vector<Window*> getWindows();
    void addWindow(Window &window);
    void removeWindow(Window &window);

  };

}

#endif//__APPLICATION_H__
