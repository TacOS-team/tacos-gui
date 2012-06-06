#include <sombrero.h>
#include <string>
#include <vector>
#include <fileinfo.h>
#include <directory.h>
#include <stdlib.h>

#define DEFAULT_FOLDER "ressources/images/wallpapers"
#define TIMER_PERIOD 1000

using namespace std;
using namespace sombrero;

class Wallpaper : public has_slots<> {
 private:
  Window w;
  list<FileInfo> fl;
  list<FileInfo>::iterator it;
  Image *i;
  Timer t;
  bool lolMode;
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
        if (this->lolMode) {
          int effect = rand() % 4;
          if (effect == 1) {
            i->applyNegativeFilter();
          } else if (effect == 2) {
            i->applyPowerfullnessOfTheFonkFilter();
          }
        }
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

  void setLolMode(bool lolMode) {
    this->lolMode = lolMode;
  }
};

int main(int argc, char **argv) {

  FileInfo *f;
  int opt;
  bool lolmode = false;
  string path = DEFAULT_FOLDER;

  while ((opt = getopt(argc, argv, "[l][d:]")) != -1) {
    switch (opt) {
      case 'l':
        lolmode = true;
        break;
      case 'd':
         path = (string &) optarg;
        break;
      default:
        fprintf(stderr, "Usage: %s [-l] [-d] directory\n",
            argv[0]);
        exit(EXIT_FAILURE);
    }
  }

    f = new FileInfo(path.c_str());

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
  wp.setLolMode (lolmode);
  srand(time(NULL));
  
  Application::getInstance()->sombrerun();

  return 0;
}
