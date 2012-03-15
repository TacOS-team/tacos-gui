#ifndef _GWINDOWS_MANAGER_H_
#define _GWINDOWS_MANAGER_H_

#include <vector>

#include <gwindow.h>
#include <pronlib.h>

using namespace std;


class GWindowsManager {
  vector<GWindow*> windowsList;
  Window rootWindow;
  PronWindowAttributes rootWindowAttributes;

  bool overlaps(GWindow *gw);
 public:
  GWindowsManager(Window rootWindow);
  void addGWindow(GWindow *w);

  // Get the guacamole window from a window id
  // matches if it is the id of the window or of the decorator
  GWindow* getGWindow(Window w);
  PronWindowAttributes & getRootWindowAttributes();

  void destroy(Window w);

  void initWindowPosition(GWindow *gw);

  bool empty();

};

#endif// _GWINDOWS_MANAGER_H_

