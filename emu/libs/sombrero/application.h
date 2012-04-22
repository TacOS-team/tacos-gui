#ifndef __APPLICATION_H__
#define __APPLICATION_H__
/**
 * @file application.h
 * Application class definition
 */
#include <vector>
#include "pronlib.h"
#include "window.h"
#include "widget.h"

namespace sombrero {

class widget;
class Window;

/**
 * @brief Main sombrero's class
 * Main sombrero's class. Start sombrero, add / remove window...
 */
class Application {

 private:
  pron::Display *d; /**< Pron display */
  static Application *instance; /**< Singleton instance of Application */
  vector<Window*> windows; /**< Application windows vector */
  /**
   * Private constructor (singleton)
   */
  Application();
  /**
   * Private destructor
   */
  ~Application();

 public:
  /**
   * Get the static instance
   * Creates it if it's NULL
   * @return Application static instance
   */
  static Application* getInstance();
  /**
   * Lanches the application sombrero
   */
  void run();
  /**
   * Get application the windows
   * @return Application windows vector
   */
  std::vector<Window*> getWindows();
  /**
   * Add a window to the application
   * @param window The window to add
   */
  void addWindow(Window &window);
  /**
   * Removes a window from the application
   * @param window The Window to remove
   */
  void removeWindow(Window &window);

  /**
   * Allows only the Widget class
   * to read my private attributes and methods
   */
  friend class Widget;
};

} // namespace sombrero

#endif//__APPLICATION_H__
