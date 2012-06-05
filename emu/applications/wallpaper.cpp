#include <sombrero.h>
#include <string>
#include <vector>
#include <fileinfo.h>
#include <directory.h>

#define DEFAULT_FOLDER "ressources/images/wallpapers"
#define TIMER_PERIOD 10000

using namespace std;
using namespace sombrero;

class Wallpaper : public has_slots<> {
 private:
  Window w;
  list<FileInfo> fl;
  list<FileInfo>::iterator it;
  Image *i;
  Timer t;
  bool imageFound;
 public:

  void changeImage() {
    printf("changeImage\n");
    bool stop = false;
    while (1) {
      if (it->getFileName().find(".jpg") != std::string::npos) {
        printf("JPG FOUND !\n");
        if (imageFound) {
          delete i;
        }
        i = new Image(it->getAbsolutePath());
        w.add(i);
        i->draw();
        imageFound = true;
        stop = true;
      }
      if (it != fl.end() ){
        ++it;
      } else if (imageFound) {
        it = fl.begin();
      } else {
        fprintf(stderr, "The is no file in the folder, exiting\n");
        exit(1);
      }
      if (stop) {
        break;
      }
    }
  }

  Wallpaper(list<FileInfo> l) 
      : w("Wallpaper", 0, 0, 800, 600, false), 
        fl(l), 
        it(fl.begin()), t(TIMER_PERIOD), imageFound(false) {
    if (fl.size() == 0) {
      fprintf(stderr, "There is no file in this folder, exiting\n");
      exit(1);
    }
    changeImage();
    t.signal.connect(this, &Wallpaper::changeImage);
    t.start();
  }

  ~Wallpaper() {
    if (imageFound) {
      delete i;
    }
  }
};

int main(int argc, char **argv) {

  FileInfo *f;

  if (argc < 2) {
    printf("using default folder : %s\n", DEFAULT_FOLDER);
    f = new FileInfo(DEFAULT_FOLDER);
  } else {
    f = new FileInfo(argv[1]);
  }

  if (!f->exists()) {
    fprintf(stderr, "error : file or folder doesn't exists");
    return 1;
  }
  
  if (!f->isDir()) {
    fprintf(stderr, "error : not a folder");
    return 1;
  }

  Directory d(f->getAbsolutePath());
  delete f;

  Wallpaper wp(d.entryInfoList());
  
  Application::getInstance()->sombrerun();

  return 0;
}
