#include <iostream>
#include <clibtacos>

#include <pronlib.h>


int main() {
  Display* display = pronConnect();
  pronSelectInput(display, display->rootWindow, CREATE_NOTIFY);
}
