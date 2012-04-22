#include "sombrero.h"
#include "unistd.h"

int main(int argc, char **argv ) {
  argc++;
  argv++;
  sombrero::init();
  sombrero::Window w(0, 0, 50, 50);
  sleep(10);
  return 0;
}
