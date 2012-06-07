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
  // Window button

  sombrero::Window w("Bouton", 0, 0, 300, 20);
  char title[] = "Press me to change the label";
  char title2[] = "Hello I am the first label";
  sombrero::Button b(title);
  w.add(&b);

  // Window label

  sombrero::Window w2("Label", 0, 0, 300, 20);
  sombrero::Label l(title2);
  w2.add(&l);

  // Clicked signatl handler
  MouseClicked mc(&l);
  
  // Connection
  b.clicked.connect(&mc, &MouseClicked::doClick);

  sombrero::Application::getInstance()->sombrerun();

  return 0;
}
