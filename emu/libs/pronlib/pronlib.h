/**
 * @file pronlib.h
 * Defines types and primitives offered by the pronlib.
 */
#ifndef _PRONLIB_H_
#define _PRONLIB_H_

#include <queue>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <utility>

#include <pron_proto.h>
#include <proto/bits/image.h>
#include <pronlib_types.h>
#include <tsock.h>

using namespace std;

#define MAX_MSG_SIZE 1024 /**< Maximum size of a message (1Kio) */

namespace pron {

/**
 * Describes a connection to pron.
 * @todo Propose an object-oriented API.
 */
class Display {
 public:
  /**
   * Constructor.
   * @param fd The file descriptor used for the connection to pron
   * @param welcome The pron Welcome message
   */
  Display(int fd, RespWelcome *welcome);

  /**
   * Generates a new unique resource id.
   * @todo Check bounds
   * @return The new resource id
   */
  int newResourceId();

  /**
   * Reads a message from pron.
   * @param msg Pointer to a PronMessage to store the message read
   * @param len The maximum length to read
   * @return The size read (-1 if an error occured)
   */
  int read(PronMessage *msg, size_t len);

  /**
   * @brief Reads a message of the given type from pron.
   * Reads a message from pron. If the type of the message is equal
   * to @a type, returns it. Else, if the message is an event, enqueues it
   * into the event queue. Else, prints an error message.
   * @param type The requested type
   * @param buffer The buffer where to store the message
   * @param len The length of the buffer
   * @return The size read (-1 if an error occured)
   */
  int read(MessageType type, void *buffer, size_t len);

  /**
   * @brief Gets the next event from pron.
   * If the events queue is not empty, returns the first event of the queue.
   * Else, read a new event from the connection to pron.
   * @param e The event returned
   * @return success
   */
  bool getNextEvent(PronEvent *e);

  int fd; /**< File descriptor used for the connection to pron */
  Window rootWindow; /**< Id of the root window */
  int startId; /**< First usable resource id */
  int endId; /**< Last usable resource id */
  int curId; /**< Current resource id */
  GC defaultGC; /**< Default graphics context */
  
 private:
  queue< pair<int, PronEvent*> > queuedEvents; /**< Events read from the server and queued */
};

/**
 * Connects to the pron server.
 * @return A pointer to the Display object describing the connection, or NULL if connection failed
 */
Display* pronConnect();

/**
 * Creates a new window.
 * @param d The connection to pron
 * @param parent The id of the the parent window
 * @param x The x coordinate of the top-left corner
 * @param y The y coordinate of the top-left corner
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
 * @param d The connection to pron
 * @param w The window to map
 */
void pronMapWindow(Display *d, Window w);

/**
 * Unmaps a window (hides it from the screen).
 * @param d The connection to pron
 * @param w The window to unmap
 */
void pronUnmapWindow(Display *d, Window w);

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
 * @return success
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
 * @return success
 */
int pronDrawLine(Display *d, Window w, GC gc, int x1, int y1, int x2, int y2);

/**
 * Sent by a client to draw a rectangle to (x,y) width height sized.
 * @todo Implement drawables instead of just windows
 * @param d The connection to pron
 * @param w The window in which to draw the rectangle
 * @param gc The graphics context to use
 * @param x The x-coordinate of the top-left corner of the rectangle
 * @param y The y-coordinate of the top-left corner of the rectangle
 * @param width The width of the rectangle
 * @param height The height of the rectangle
 * @return success
 */
int pronDrawRect(Display *d, Window w, GC gc, int x, int y, int width, int height);

/**
 * Sent by a client to fill a rectangle to (x,y) width height sized.
 * @todo Implement drawables instead of just windows
 * @param d The connection to pron
 * @param w The window in which to draw the rectangle
 * @param gc The graphics context to use
 * @param x The x-coordinate of the top-left corner of the rectangle
 * @param y The y-coordinate of the top-left corner of the rectangle
 * @param width The width of the rectangle
 * @param height The height of the rectangle
 * @return success
 */
int pronFillRectangle(Display *d, Window w, GC gc, int x, int y, int width, int height);

/**
 * Sent by a client to draw a circle with given center (x,y) and radius.
 * @todo Implement drawables instead of just windows
 * @param d The connection to pron
 * @param w The window in which to draw the line
 * @param gc The graphics context to use
 * @param x The x-coordinate of the top-left corner of the circle
 * @param y The y-coordinate of the top-left corner of the circle
 * @param radius The radius of the circle
 * @return success
 */
int pronDrawCircle(Display *d, Window w, GC gc, int x, int y, int radius);

/**
 * Sent by a client to draw a filled circle with given center (x,y) and radius.
 * @todo Implement drawables instead of just windows
 * @param d The connection to pron
 * @param w The window in which to draw the line
 * @param gc The graphics context to use
 * @param x The x-coordinate of the top-left corner of the circle
 * @param y The y-coordinate of the top-left corner of the circle
 * @param radius The radius of the circle
 * @return success
 */
int pronFillCircle(Display *d, Window w, GC gc, int x, int y, int radius);

/**
 * Closes the connection to pron.
 * @param d The connection to pron
 */
void pronDisconnect(Display *d);

/**
 * Subscribes to the events associated with the specified event mask.
 * @param d The connection to pron
 * @param w The window whose events to subscribe
 * @param eventMask The event mask
 */
void pronSelectInput(Display *d, Window w, uint32_t eventMask);

/**
 * Tells pron not to propagate the events associated with the specified event mask.
 * @param d The connection to pron
 * @param w The window whose events not to propagate
 * @param eventMask The event mask
 */
void pronDontPropagateEvent(Display *d, Window w, uint32_t eventMask);

/**
 * Waits for a new event.
 * @param d The connection to pron
 * @param e The returned event
 * @param nonBlocking Whether the function should block or not if there is no
 * available event.
 * @return success
 */
int pronNextEvent(Display *d, PronEvent *e, bool nonBlocking = false);

/**
 * Gets the attributes of a given window.
 * @param d The connection to pron
 * @param w The window whose attributes to get
 * @param attr The attributes of the widow
 */
void pronGetWindowAttributes(Display * d, Window w, PronWindowAttributes * attr);

/**
 * Sets the attributes of a given window.
 * @param d The connection to pron
 * @param w The window whose attributes to set
 * @param newAttr The new attributes structure
 * @param mask The mask that specifies the attributes to be set
 */
void pronSetWindowAttributes(Display * d, Window w, const PronWindowAttributes &  newAttr, unsigned int mask); 

/**
 * Reparents a window.
 * @param d The connection to pron
 * @param w The window to reparent
 * @param newParent The id of the new parent window
 */
void pronReparentWindow(Display *d, unsigned int w, unsigned int newParent);  

/**
 * Destroys a window.
 * @param d The connection to pron
 * @param w The window to destroy
 */
void pronDestroyWindow(Display *d, unsigned int w);

/**
 * Moves a window of x pixels on the x-axis and y pixels on the y-axis.
 * @param d The connection to pron
 * @param w The window to move
 * @param x The relative move on the x-axis
 * @param y The relative move on the y-axis
 */
void pronMoveWindow(Display *d, unsigned int w, int x, int y);

/**
 * Moves a window at (x,y) position.
 * @param d The connection to pron
 * @param w The window to move
 * @param x The new x-coordinate
 * @param y The new y-coordinate
 */
void pronMoveWindowTo(Display *d, unsigned int w, int x, int y);

/**
 * Puts an image into a window.
 * @todo put on any drawable (pixmap & window)
 * @param d The connection to pron
 * @param w The window in which to put the image
 * @param gc The graphics context to use
 * @param image The PronImage to put
 * @param srcX The x-coordinate of the top-left corner of the image area to put
 * @param srcY The y-coordinate of the top-left corner of the image area to put
 * @param width The width of the image area to put
 * @param height The height of the image area to put
 * @param destX The destination top-left corner x-coordinate
 * @param destY The destination top-left corner y-coordinate
 */
void pronPutImage(Display *d, Window w, GC __attribute__((unused)) gc, PronImage *image, 
    int srcX, int srcY, int width, int height, int destX, int destY);  

/**
 * Resizes a window.
 * @param d The connection to pron
 * @param w The window to resize
 * @param width The new width
 * @param height The new height
 */
void pronResizeWindow(Display *d, unsigned int w, int width, int height);

/**
 * Creates a new pixmap.
 * @param d The connection to pron
 * @param width The width
 * @param height The height
 * @param depth of the pixmap
 * @return The pixmap id of the created pixmap
 */
Pixmap pronCreatePixmap(Display *d, int width, int height, int depth);

/**
 * Frees a pixmap.
 * @param d The connection to pron
 * @param p The pixmap to free
 */
void pronFreePixmap(Display *d, unsigned int p);

/**
 * Copies an area from a drawable to another one.
 * @param d The connection to pron
 * @param src The source drawable
 * @param dest The destination drawable
 * @param gc The graphics context to use during the copy
 * @param srcX The x-coordinate of the top-left corner of the
 * source drawable area to copy
 * @param srcY The y-coordinate of the top-left corner of the
 * source drawable area to copy
 * @param width The width of the source drawable area to copy
 * @param height The height of the source drawable area to copy
 * @param destX The destination top-left corner x-coordinate
 * @param destY The destination top-left corner y-coordinate
 */
void pronCopyArea(Display *d, Drawable src, Drawable dest, GC gc, 
    int srcX, int srcY, unsigned int width, unsigned int height, int destX, int destY);

/**
 * Allocates memory needed to receive a pron event.
 * @return Pointer to the allocated PronEvent
 */
PronEvent* getPronEvent();

} // namespace pron

#endif
