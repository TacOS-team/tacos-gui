#include <errno.h>
#include <pronlib.h>
#include <sombrero.h>
#include <map>
#include <elf.h>

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
  sombrero::Grid *grid;

  void windowCreated(pron::EventWindowCreated *e) {
    printf("Window created! (%x)\n", e->window);
    if (e->parent == this->d->rootWindow && (e->window >> 16) != 0x01) {
      printf("top window\n");
      windows[e->window] = new WindowItem(e->window, e->attributes.wm_title);
      this->grid->add(&(windows[e->window]->getButton()));
      
      map<pron::Window, sombrero::Widget*>::iterator it = this->widgets.find(e->window);
      if (it != this->widgets.end()) {
        it->second->subscribeEvent(pron::EV_DESTROY_WINDOW);
        it->second->subscribeEvent(pron::EV_KEY_PRESSED);
        it->second->subscribeEvent(pron::EV_KEY_RELEASED);
      } else { 
        pron::pronSelectInput(this->d, e->window, PRON_EVENTMASK(pron::EV_DESTROY_WINDOW) | PRON_EVENTMASK(pron::EV_KEY_PRESSED) | PRON_EVENTMASK(pron::EV_KEY_RELEASED));
      }
    }
  }

  void windowDestroyed(pron::EventDestroyWindow *e) {
    printf("Window destroyed! (%x)\n", e->window);
    map<pron::Window, WindowItem*>::iterator it = this->windows.find(e->window);
    if (it != this->windows.end()) {
      this->grid->remove(&(it->second->getButton()));
      delete it->second;
      windows.erase(it);
    }
  }

  void keyPressed(pron::EventKeyPressed *e __attribute__((unused))) {
    if (e->keysym == pron::PRONK_F1 && (e->modifiers & pron::KMOD_LCTRL)) {
      string path("build/applications/velo");
      exec_elf((char *)path.c_str(), 0);
    }
  }

  void keyReleased(pron::EventKeyReleased *e __attribute__((unused))) {
    printf("KEY RELEASED\n");
  }

 public:
  void sombrerun() {
    this->grid = new sombrero::Grid();

    sombrero::Window w("Panel", 0, 600 - 20, 800, 20, false);
    w.add(this->grid);
    
    this->grid->clear();

    pron::pronSelectInput(this->d, this->d->rootWindow,
        PRON_EVENTMASK(pron::EV_WINDOW_CREATED) | PRON_EVENTMASK(pron::EV_DESTROY_WINDOW) | PRON_EVENTMASK(pron::EV_KEY_PRESSED) | PRON_EVENTMASK(pron::EV_KEY_RELEASED));

    Application::sombrerun();
  }
};

int main() {
  printf("--- panel ---\n");
  AppPanel panel;
  printf("--- end panel ---\n");
  panel.sombrerun();

  return 0;
}
