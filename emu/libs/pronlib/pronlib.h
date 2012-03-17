/**
 * @file pronlib.h
 * Defines types and primitives offered by the pronlib.
 */
#ifndef _PRONLIB_H_
#define _PRONLIB_H_

#include <pron_proto.h>
#include <pronlib_types.h>
#include <stdint.h>
#include <queue>
#include <utility>
#include <tsock.h>
#include <string.h>
#include <cstdio>

using namespace std;

#define MAX_MSG_SIZE 1024 /**< max size of a message (1Kio) */

struct Display;

/**
 * Connects to the pron server.
 * @return A pointer to the Display object describing the connection, or NULL if connection failed
 */
Display* pronConnect();

/**
 * Creates a new window.
 * @param d The connection to pron
 * @param parent The id of the the parent window
 * @param x The x coordinate
 * @param y The y coordinate
 * @param width The width
 * @param height The height
 * @return The window id of the created window
 */
Window pronCreateWindow(Display *d, Window parent, int x, int y, int width, int height);

/**
 * Clears the specified window.
 * @param d The connection to pron
 * @param w The id of the window to clear
 */
void pronClearWindow(Display *d, Window w);

/**
 * Creates a new graphics context.
 * @param d The connection to pron
 * @param values The values to set in the new GC
 * @param mask The mask that specifies the values to be set
 * @return The id of the created GC
 */
GC pronCreateGC(Display *d, const PronGCValues &values, unsigned int mask);

/**
 * Gets the values of a specified graphics context.
 * @param d The connection to pron
 * @param gc The graphics context whose values to get
 * @param values The values of the graphics context
 */
void pronGetGCValues(Display *d, GC gc, PronGCValues *values);

/**
 * Changes the values of a specified graphics context.
 * @param d The connection to pron
 * @param gc The graphics context whose values to set
 * @param values The values to set
 * @param mask The mask that specifies the values to be set
 */
void pronChangeGC(Display *d, GC gc, const PronGCValues &values, unsigned int mask);

/**
 * Destroys a graphics context.
 * @param d The connection to pron
 * @param gc The graphics context to destroy
 */
void pronFreeGC(Display *d, GC gc);

/**
 * Maps a window (shows it on the screen).
 * @todo Implement this
 * @param d The connection to pron
 * @param w The window to map
 */
void pronMapWindow(Display *d, Window w);

/**
 * Raises a window (moves it on top of the screen).
 * @param d The connection to pron
 * @param w The window to raise
 */
void pronRaiseWindow(Display *d, Window w);

/**
 * Draws a point at (x, y).
 * @todo Implement drawables instead of just windows
 * @param d The connection to pron
 * @param w The window in which to draw the line
 * @param gc The graphics context to use
 * @param x The x-coordinate of the point
 * @param y The y-coordinate of the point
 */
int pronDrawPoint(Display *d, Window w, GC gc, int x, int y);

/**
 * Draws a line between (x1, y1) and (x2, y2).
 * @todo Implement drawables instead of just windows
 * @param d The connection to pron
 * @param w The window in which to draw the line
 * @param gc The graphics context to use
 * @param x1 The x-coordinate of the first point to join
 * @param y1 The y-coordinate of the first point to join
 * @param x2 The x-coordinate of the second point to join
 * @param y2 The y-coordinate of the second point to join
 */
int pronDrawLine(Display *d, Window w, GC gc, int x1, int y1, int x2, int y2);

/**
 * Sent by a client to draw a rectangle to (x,y) width height sized.
 * @todo Implement drawables instead of just windows
 * @param d The connection to pron
 * @param w The window in which to draw the line
 * @param gc The graphics context to use
 * @param x The x-coordinate of the point to join
 * @param y The y-coordinate of the point to join
 * @param width of the rectangle
 * @param height of the rectangle
 */
int pronDrawRect(Display *d, Window w, GC gc, int x, int y, int width, int height);

/**
 * Sent by a client to fill a rectangle to (x,y) width height sized.
 * @todo Implement drawables instead of just windows
 * @param d The connection to pron
 * @param w The window in which to draw the line
 * @param gc The graphics context to use
 * @param x The x-coordinate of the point to join
 * @param y The y-coordinate of the point to join
 * @param width of the rectangle
 * @param height of the rectangle
 */
int pronFillRectangle(Display *d, Window w, GC gc, int x, int y, int width, int height);

/**
 * Sent by a client to draw a circle with given center (x,y) and radius
 * @todo Implement drawables instead of just windows
 * @param d The connection to pron
 * @param w The window in which to draw the line
 * @param gc The graphics context to use
 * @param x The x-coordinate of the point to join
 * @param y The y-coordinate of the point to join
 * @param radius of the circle
 */
int pronDrawCircle(Display *d, Window w, GC gc, int x, int y, int radius);

/**
 * Sent by a client to draw a filled circle with given center (x,y) and radius
 * @todo Implement drawables instead of just windows
 * @param d The connection to pron
 * @param w The window in which to draw the line
 * @param gc The graphics context to use
 * @param x The x-coordinate of the point to join
 * @param y The y-coordinate of the point to join
 * @param radius of the circle
 */
int pronFillCircle(Display *d, Window w, GC gc, int x, int y, int radius);


/**
 * Closes the connection to pron.
 * @param d The connection to pron
 */
void pronDisconnect(Display *d);

/**
 * Subscribes to the events associated with the specified event mask.
 * @todo Implement this
 * @param d The connection to pron
 * @param w The window whose events to subscribe
 * @param event_mask The event mask
 */
void pronSelectInput(Display *d, Window w, uint32_t eventMask);

/**
 * Tells pron not to propagate the events associated with the specified event mask.
 * @param d The connection to pron
 * @param w The window whose events not to propagate
 * @param event_mask The event mask
 */
void pronDontPropagateEvent(Display *d, Window w, uint32_t eventMask);

/**
 * Wait a new event.
 * @param d The connection to pron
 * @param e Event returned
 */
int pronNextEvent(Display *d, PronEvent *e, bool nonBlocking = false);

/**
 * Get the attributes of a given window.
 * @param d The connection to pron
 * @param w The window whose attributes to get
 * @param attr The attributes of the widow
 */
void pronGetWindowAttributes(Display * d, Window w, PronWindowAttributes * attr);

/**
 * Set the attributes of a given window.
 * @param d The connection to pron
 * @param w The window whose attributes to get
 * @param newAttr The new attributes structure
 * @param mask The mask that indiquates the attributes to be set
 */
void pronSetWindowAttributes(Display * d, Window w, const PronWindowAttributes &  newAttr, unsigned int mask); 

/**
 * Reparent a Window
 * @param d The connection to pron
 * @param w The window
 * @param newParent The new parent window of w
 */
void pronReparentWindow(Display *d, unsigned int w, unsigned int newParent);  

/**
 * Destroy a Window
 * @param d The connection to pron
 * @param w The window
 */
void pronDestroyWindow(Display *d, unsigned int w);   

/**
 * Move a Window of x pixels on x and y pixels on y
 * @param d The connection to pron
 * @param w The window
 * @param x The relative move on x
 * @param y The relative move on y
 */
void pronMoveWindow(Display *d, unsigned int w, int x, int y);

/**
 * Describes a connection to pron.
 * TODO: move to a real class and propose an object-oriented API.
 */
struct Display {
  /** Constructor. */
  Display(int fd, RespWelcome *welcome) {
    this->fd = fd;
    this->rootWindow = welcome->rootWindow;
    this->startId = welcome->startId;
    this->endId = welcome->endId;
    this->curId = this->startId;
    PronGCValues values;
    COLOR(values.fg, 24).r = 255;
    COLOR(values.fg, 24).g = 77;
    COLOR(values.fg, 24).b = 182;
    COLOR(values.bg, 24).r = 0;
    COLOR(values.bg, 24).g = 0;
    COLOR(values.bg, 24).b = 0;
    this->defaultGC = pronCreateGC(this, values, GC_VAL_FG | GC_VAL_BG);
  }

  /**
   * Generates a new unique resource id.
   * @todo check bounds
   * @return the new resource id
   */
  int newResourceId() {
    return this->curId++;
  }

  int read(PronMessage *msg, size_t len) {
    int sizeRead = tsock_read(this->fd, msg, MAX_MSG_SIZE);

    if (sizeRead < 0) {
      // TODO: handle read error?
      //perror("Failed to read from server");
    } else if (sizeRead == 0) {
      // TODO: handle server disconnection
      fprintf(stderr, "Server has closed the connection\n");
    } else if ((msg->type & ER_PREFIX) == ER_PREFIX) {
      // TODO: trigger error handler
      fprintf(stderr, "Received error message from server: %x\n", msg->type);
    }

    return sizeRead;
  }

  int read(MessageType type, void *buffer, size_t len) {
    PronMessage *msgRead;
    int sizeRead;
    bool stop = false;

    while (!stop) {
      msgRead = (PronMessage*) malloc(MAX_MSG_SIZE);
      sizeRead = this->read(msgRead, MAX_MSG_SIZE);

      if (sizeRead > 0) {
        if (msgRead->type == type) {
          memcpy(buffer, msgRead, len);
          free(msgRead);
          stop = true;
        } else if ((msgRead->type & EV_PREFIX) == EV_PREFIX) {
          printf("Queuing event...\n");
          this->queuedEvents.push(pair<int, PronEvent*>(sizeRead, (PronEvent*) msgRead));
        } else {
          fprintf(stderr, "Wrong message type (expected %x, got %x)\n", type, msgRead->type);
          free(msgRead);
          stop = true;
        }
      }  
    }

    return sizeRead;
  }

  bool getNextEvent(PronEvent *e) {
    bool ok = false;

    if (!this->queuedEvents.empty()) {
      memcpy(e, this->queuedEvents.front().second, this->queuedEvents.front().first);
      free(this->queuedEvents.front().second);
      this->queuedEvents.pop();
      ok = true;
    } else {
      if (this->read(e, MAX_MSG_SIZE) > 0) {
        if ((e->type & EV_PREFIX) == EV_PREFIX) {
          ok = true;
        } else {
          fprintf(stderr, "Wrong message type (expected event, got %x)\n", e->type);
        }
      }
    }

    return ok;
  }

  int fd; /**< file descriptor used for the connection to pron */
  Window rootWindow; /**< id of the root window */
  int startId; /**< first usable resource id */
  int endId; /**< last usable resource id */
  int curId; /**< current resource id */
  GC defaultGC; /**< default graphic context */
  queue< pair<int, PronEvent*> > queuedEvents; /**< events read from the server and queued */
};

#endif
