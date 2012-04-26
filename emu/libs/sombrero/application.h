#ifndef __APPLICATION_H__
#define __APPLICATION_H__
/**
 * @file application.h
 * Application class definition
 */
#include <vector>
#include <map>
#include "pronlib.h"

namespace sombrero {

class widget;
class Window;

/**
 * @brief Main sombrero's class
 * Main sombrero's class. Start sombrero, add / remove window...
 */
class Application {

 private:
  static Application *instance; /**< Singleton instance of Application */
  /**
   * Private constructor (singleton)
   */
  Application();
  /**
   * Private destructor
   */
  ~Application();

 public:
  pron::Display *d; /**< Pron display */
  std::map<pron::Window, Widget*> widgets; /**< A map with every pron::Window associated to a widget */
  
  /**
   * Get the static instance
   * Creates it if it's NULL
   * @return Application static instance
   */
  static Application* getInstance();
  /**
   * Initialize Sombrero
   */
  void init();
  /**
   * Lanches the application sombrero
   */
  void sombrerun();
};

} // namespace sombrero

#endif//__APPLICATION_H__
