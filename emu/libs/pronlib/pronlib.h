#ifndef _PRONLIB_H_
#define _PRONLIB_H_

#include <pron_types.h>

#include <stdint.h>

Display* pronConnect();

Window pronCreateWindow(Display *d, Window parent, int x, int y, int width, int height);

GC pronCreateGC(Display *d);

void pronMapWindow(Display *d, Window w);

void pronDrawLine(Display *d, Window w, GC gc, int x1, int y1, int x2, int y2);

void pronDisconnect(Display *d);

void pronSubscribeEvent(Display *d, uint32_t events);

#endif
