#include <iostream>
#include <clibtacos>

#include <pronlib.h>


int main() {
  Display* display = pronConnect();
  pronSubscribeEvent(display, CREATE_NOTIFY);
}
