#include <pronlib.h>

using namespace pron;

int main() {
  // Connect to pron
  Display *d = pronConnect();

  if (d == NULL) {
    fprintf(stderr, "Unable to connect to pron.\n");
    exit(1);
  }

  // Create window
  Window w = pronCreateWindow(d, d->rootWindow, 0, 0, 320, 240);

  // Subscribe to expose, destroy and resize events
  pronSelectInput(d, w, PRON_EVENTMASK(EV_EXPOSE) | PRON_EVENTMASK(EV_DESTROY_WINDOW) | PRON_EVENTMASK(EV_RESIZE_WINDOW));

  // Map the window (show it on the screen)
  pronMapWindow(d, w);

  // Allocate a pronEvent
  PronEvent *event = getPronEvent();

  bool stop = false;

  while (!stop) {
    // Get the next pronEvent
    if (!pronNextEvent(d, event)) {
      fprintf(stderr, "pron has closed the connection.\n");
      exit(1);
    }

    // Handle the event
    switch (event->type) {
      case EV_EXPOSE: {
        EventExpose *exposeEvent = (EventExpose*) event;
        // ...
        break;
      }
      case EV_DESTROY_WINDOW: {
        EventDestroyWindow *destroyWindowEvent = (EventDestroyWindow*) event;
        // ...
        break;
      }
      case EV_RESIZE_WINDOW: {
        EventResizeWindow *resizeWindowEvent = (EventResizeWindow*) event;
        // ...
        break;
      }
      default:
        break;
    }
  }

  // Destroy the window
  pronDestroyWindow(d, w);

  // Disconnect from pron
  pronDisconnect(d);

  return 0;
}
