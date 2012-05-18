#include "sombrero.h"
#include "unistd.h"
#include "image.h"
#include <cstdio>
#include <cstdlib>

int main(int argc, char **argv) {
  
  if (argc < 2) {
    printf("usage : sombrero_image <filename>\n");
    return 1;
  }
 
  // Initialize sombrero 
  sombrero::Application::getInstance()->init();
  
  // Window button
  string filename(argv[1]);
  sombrero::Image i(filename);
  sombrero::Window w(0, 0, i.getImageWidth(), i.getImageHeight());
  w.add(&i);

  sombrero::Application::getInstance()->sombrerun();

  return 0;
}
