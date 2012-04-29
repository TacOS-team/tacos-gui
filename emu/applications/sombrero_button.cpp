#include "sombrero.h"
#include "unistd.h"
#include <cstdio>

class MouseClicked : public has_slots<> {
  public:
    void doClick() {
      printf("Clicked noob lolilol!\n");
    }
};

int main() {
  sombrero::Application::getInstance()->init();
  sombrero::Window w(0, 0, 200, 200);
  char title[8] = "LOLILOL";
  sombrero::Button b(&w, title);

  // Clicked signatl handler
  MouseClicked mc;
  mc.doClick();
  
  // Connection
  b.clicked.connect(&mc, &MouseClicked::doClick);

  sombrero::Application::getInstance()->sombrerun();

  return 0;
}
