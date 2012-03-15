#ifndef __KEYBOARD_H__
#define __KEYBOARD_H__

class Keyboard {
public:
  int fd;

  Keyboard();

  void checkEvents();
};

#endif
