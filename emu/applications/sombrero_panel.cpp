#include <errno.h>
#include <pronlib.h>
#include <sombrero.h>
#include <map>

class AppPanel : public sombrero::Application {
 private:
  class WindowItem : public has_slots<> {
    private:
      pron::Window pronWindow;
      std::string title;
      sombrero::Button button;

    public:
      WindowItem(pron::Window pronWindow, char *title)
          : pronWindow(pronWindow), title(title), button(title) {
        this->button.clicked.connect(this, &WindowItem::buttonClicked); 
      }

      sombrero::Button& getButton() {
        return this->button;
      }

      void buttonClicked() {
        pron::pronRaiseWindow(Application::getInstance()->d, this->pronWindow);
      }
  };

  std::map<pron::Window, WindowItem*> windows;
  sombrero::Grid grid;

  void windowCreated(pron::EventWindowCreated *e) {
    printf("Window created! (%x)\n", e->window);
    if (e->parent == this->d->rootWindow && (e->window >> 16) != 0x01) {
      printf("top window\n");
      windows[e->window] = new WindowItem(e->window, e->attributes.wm_title);
      this->grid.add(&(windows[e->window]->getButton()));
      
      map<pron::Window, sombrero::Widget*>::iterator it = this->widgets.find(e->window);
      if (it != this->widgets.end()) {
        it->second->subscribeEvent(pron::EV_DESTROY_WINDOW);
      } else { 
        pron::pronSelectInput(this->d, e->window, PRON_EVENTMASK(pron::EV_DESTROY_WINDOW));
      }
    }
  }

  void windowDestroyed(pron::EventDestroyWindow *e) {
    printf("Window destroyed! (%x)\n", e->window);
    map<pron::Window, WindowItem*>::iterator it = this->windows.find(e->window);
    if (it != this->windows.end()) {
      this->grid.remove(&(it->second->getButton()));
      delete it->second;
      windows.erase(it);
    }
  }

 public:
  std::map<pron::Window, sombrero::Widget*>& widgets;

  AppPanel()
      : widgets(Application::getInstance()->widgets) {
  }

  void main() {
    this->init();

    sombrero::Window w("Panel", 0, 600 - 20, 800, 20, false);
    w.add(&this->grid);
    
    this->grid.clear();

    this->sombrerun();
  }

  void sombrerun() {
    pron::PronEvent *e = pron::getPronEvent();

    pron::pronSelectInput(this->d, this->d->rootWindow,
        PRON_EVENTMASK(pron::EV_WINDOW_CREATED) | PRON_EVENTMASK(pron::EV_DESTROY_WINDOW));

    while (1) {
      if (!pron::pronNextEvent(this->d, e)) {
        if (errno == EINTR) {
          continue;
        }
        fprintf(stderr, "lol pron has closed the connection.\n");
        exit(1);
      }

      if (e->type == pron::EV_WINDOW_CREATED) {
        this->windowCreated((pron::EventWindowCreated*) e);
      } else if (e->type == pron::EV_DESTROY_WINDOW) {
        this->windowDestroyed((pron::EventDestroyWindow*) e);
      }
      
      /*printf("DEBUG éléments map application LOL\n");
      for (map<pron::Window, sombrero::Widget*>::iterator it = this->widgets.begin(); it != this->widgets.end(); ++it) {
        printf("New element %x -> %p \n", it->first, it->second);
      }*/

      map<pron::Window, sombrero::Widget*>::iterator it = this->widgets.find(e->window);
      if (it != this->widgets.end()) {
        sombrero::Widget *w = it->second;
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
};

int main() {
  printf("--- panel ---\n");
  AppPanel panel;
  printf("--- end panel ---\n");
  panel.main();

  return 0;
}
