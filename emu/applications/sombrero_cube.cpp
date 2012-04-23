#include "sombrero.h"
#include "unistd.h"

int main() {
  sombrero::init();
  sombrero::Window w(0, 0, 50, 50);
  sombrero::Canvas c(&w);
  sleep(10);
  return 0;
}
