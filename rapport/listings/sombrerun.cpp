/* 
 * Call every handleEvent*** method for the given events
 * handleEvent*** can be redefined in the child classes
 */
void Application::sombrerun() {
  pron::PronEvent *e = pron::getPronEvent();

  while (1) {
    if (!pron::pronNextEvent(this->d, e)) {
      fprintf(stderr, "pron has closed the connection, exiting.\n");
      exit(1);
    }

    map<pron::Window, Widget*>::iterator it = 
      Application::getInstance()->widgets.find(e->window);
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
