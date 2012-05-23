#include "sombrero.h"
#include "grid.h"

class MyCheckbox : public sombrero::Checkbox, public has_slots<> {
  public :
    MyCheckbox (sombrero::Container * parent) : sombrero::Checkbox(parent) {
    }
  /*  void handleEventMouseButton(pron::EventMouseButton *e) {
      printf("clic : %d %d\n", e->x, e->y);
    }
    */

};
int main () {

  sombrero::Application::getInstance()->init();
  sombrero::Window w(0,0,300,300);
  sombrero::Grid g(&w);
  MyCheckbox cb(&g);

  sombrero::Application::getInstance()->sombrerun();

  return 0;
}
