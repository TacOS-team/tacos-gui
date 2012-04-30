#include "sombrero.h"
#include "unistd.h"
#include <cstdio>
#include <cstdlib>

char newTitle[] = "NOOBIFICATION";

class MouseClicked : public has_slots<> {
  private:
    sombrero::Label *l;
  public:
    MouseClicked(sombrero::Label *l) : l(l) {}
    void doClick() {
      l->setText(newTitle);
      l->draw();
    }
};

int main() {
  sombrero::Application::getInstance()->init();
  
  // Window button

  sombrero::Window w(0, 0, 300, 20);
  char title[] = "Press me to change the label";
  char title2[] = "Hello I am the first label";
  sombrero::Button b(&w, title);

  // Window label

  sombrero::Window w2(0, 0, 300, 20);
  sombrero::Label l(&w2, title2);  

  // Clicked signatl handler
  MouseClicked mc(&l);
  
  // Connection
  b.clicked.connect(&mc, &MouseClicked::doClick);

  sombrero::Application::getInstance()->sombrerun();

  return 0;
}
