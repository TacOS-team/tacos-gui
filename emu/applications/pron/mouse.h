#ifndef __MOUSE_H__
#define __MOUSE_H__

class Mouse {
public:
  int fd;

  Mouse();

  void checkEvents();
};

#endif
