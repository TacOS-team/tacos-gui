#include <sombrero.h>
#include <string>

using namespace std;
using namespace sombrero;

int main() {
  Window w("Wallpaper", 0, 0, 800, 600, false);
  string filename = "wallpaper.jpg";
  Image img(filename);
  w.add(&img);
  img.draw();

  Application::getInstance()->sombrerun();

  return 0;
}
