void Application::sombrerun() {
  pron::PronEvent *e = pron::getPronEvent();

  // Boucle d'evenements
  bool stop = false;
  while (!stop) {
    if (!pron::pronNextEvent(this->d, e)) {
      fprintf(stderr, "pron has closed the connection, exiting.\n");
      exit(1);
    }
    
    // Evenements "generaux"
    if (e->type == pron::EV_WINDOW_CREATED) {
      this->windowCreated((pron::EventWindowCreated*) e);
    } else if (e->type == pron::EV_DESTROY_WINDOW) {
      this->windowDestroyed((pron::EventDestroyWindow*) e);
    } else if (e->type == pron::EV_KEY_PRESSED) {
      this->keyPressed((pron::EventKeyPressed*) e);
    } else if (e->type == pron::EV_KEY_RELEASED) {
      this->keyReleased((pron::EventKeyReleased*) e);
    }
    
    // Evenements sur les widgets
    map<pron::Window, Widget*>::iterator it = this->widgets.find(e->window);
    if (it != this->widgets.end()) {
      Widget *w = it->second;
      switch (e->type) {
        case pron::EV_WINDOW_CREATED:
          w->handleEventWindowCreated();
          break;
        case pron::EV_EXPOSE:
          w->handleEventExpose();
          break;
        // ...
      }
    }
  }
}
