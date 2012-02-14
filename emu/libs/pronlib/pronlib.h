/**
 * @file pronlib.h
 * Defines types and primitives offered by the pronlib.
 */
#ifndef _PRONLIB_H_
#define _PRONLIB_H_

#include <pron_proto.h>
#include <pronlib_types.h>
#include <stdint.h>

/** Describes a connection to pron. */
struct Display {
  /** Constructor. */
  Display(int fd, RespWelcome *welcome) {
    this->fd = fd;
    this->rootWindow = welcome->rootWindow;
    this->startId = welcome->startId;
    this->endId = welcome->endId;
    this->curId = this->startId;
  }

  /**
   * Generates a new unique resource id.
   * @todo check bounds
   * @return the new resource id
   */
  int newResourceId() {
    return this->curId++;
  }

  int fd; /**< file descriptor used for the connection to pron */
  Window rootWindow; /**< id of the root window */
  int startId; /**< first usable resource id */
  int endId; /**< last usable resource id */
  int curId; /**< current resource id */
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
 * @todo Implement this
 * @param d The connection to pron
 * @return The id of the created GC
 */
GC pronCreateGC(Display *d);

/**
 * Maps a window (shows it on the screen).
 * @todo Implement this
 * @param d The connection to pron
 * @param w The window to map
 */
void pronMapWindow(Display *d, Window w);

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
 * Fills a rectangle between (x1, y1) and (x2, y2).
 * @todo Implement drawables instead of just windows
 * @param d The connection to pron
 * @param w The window in which to draw the line
 * @param gc The graphics context to use
 * @param x1 The x-coordinate of the first point to join
 * @param y1 The y-coordinate of the first point to join
 * @param x2 The x-coordinate of the second point to join
 * @param y2 The y-coordinate of the second point to join
 */
int pronFillRectangle(Display *d, Window w, GC gc, int x1, int y1, int x2, int y2);

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
void pronSelectInput(Display *d, Window w, uint32_t event_mask);

/**
 * Wait a new event.
 * @todo Implement this
 * @param d The connection to pron
 * @param e Event returned
 */
void pronNextEvent(Display *d, PronEvent * e);

/**
 * Subscribes to the events associated with the specified event mask.
 * @todo Implement this
 * @param d The connection to pron
 */
PronWindowAttributes pronGetWindowAttributes(Display *d, Window w);

#endif
