/**
 * @file pronlib.cpp
 * Implementation of the pronlib.
 */
#include <cstdio>
#include <pronlib.h>
#include <tsock.h>
#include <unistd.h>

#define MAX_MSG_SIZE 1024 /**< max size of a message (1Kio) */

Display* pronConnect() {
  int fd = tsock_connect("/tmp/pron.sock");
  if (fd < 0) {
    return NULL;
  }
  printf("Sending hello...\n");
  RqHello rq(42);
  tsock_write(fd, &rq, sizeof(rq));

  printf("Reading welcome...\n");
  //int lRead;
  char buf[MAX_MSG_SIZE];
  /*while ((lRead = tsock_read(fd, buf, sizeof(buf))) < 0) {
    usleep(100000);
  }*/
  tsock_read(fd, buf, sizeof(buf));

  RespWelcome *resp = (RespWelcome*) buf;
  printf("Welcome from server [%x - %x, root = %d]\n", resp->startId, resp->endId, resp->rootWindow);
  Display *d = new Display(fd, resp);

  return d; 
}

Window pronCreateWindow(Display *d, Window parent, int x, int y, int width, int height) {
  Window w = d->newResourceId();
  RqCreateWindow rq(w, parent, x, y, width, height);
  tsock_write(d->fd, &rq, sizeof(rq));

  return w;
}

void pronClearWindow(Display *d, Window w) {
  RqClearWindow rq(w);
  tsock_write(d->fd, &rq, sizeof(rq));
}

GC pronCreateGC(Display *d) {
  GC gc = 0;
  return gc;
}

void pronMapWindow(Display *d, Window w) {
  RqMapWindow rq(w);
  tsock_write(d->fd, &rq, sizeof(rq));
}

void pronUnmapWindow(Display *d, Window w) {
  RqUnmapWindow rq(w);
  tsock_write(d->fd, &rq, sizeof(rq));
}

void pronRaiseWindow(Display *d, Window w) {
  RqRaiseWindow rq(w);
  tsock_write(d->fd, &rq, sizeof(rq));
}

int pronDrawLine(Display *d, Window w, GC gc, int x1, int y1, int x2, int y2) {
  RqDrawLine rq(gc, w, x1, y1, x2, y2);
  return (tsock_write(d->fd, &rq, sizeof(rq)) > 0);
}

int pronDrawRect(Display *d, Window w, GC gc, int x, int y, int width, int height) {
  RqDrawRect rq(gc, w, x, y, width, height);
  return (tsock_write(d->fd, &rq, sizeof(rq)) > 0);
}

int pronFillRectangle(Display *d, Window w, GC gc, int x, int y, int width, int height) {
  RqFillRectangle rq(gc, w, x, y, width, height);
  return (tsock_write(d->fd, &rq, sizeof(rq)) > 0);
}

int pronDrawCircle(Display *d, Window w, GC gc, int x, int y, int radius) {
  RqDrawCircle rq (gc, w, x, y, radius);
  return (tsock_write(d->fd,&rq,sizeof(rq)) > 0);
}

int pronFillCircle(Display *d, Window w, GC gc, int x, int y, int radius){
  RqFillCircle rq (gc, w, x, y, radius);
  return (tsock_write(d->fd,&rq,sizeof(rq)) > 0);
}

void pronGetWindowAttributes(Display * d, Window w, PronWindowAttributes * attr) {
  RqGetWindowAttributes rq(w);
  tsock_write(d->fd, &rq, sizeof(rq)) ;
  void * buffer = malloc(sizeof(RespWindowAttributes));
  /*while (tsock_read(d->fd, buffer, sizeof(RespWindowAttributes)) < 0) {
    usleep(100000);
  }*/
  tsock_read(d->fd, buffer, sizeof(RespWindowAttributes));
  *attr = ((RespWindowAttributes*) buffer)->attributes;
}

void pronSetWindowAttributes(Display * d, Window w, const PronWindowAttributes & newAttr, unsigned int mask) {  
  RqSetWindowAttributes rq(w,newAttr,mask);
  tsock_write(d->fd,&rq,sizeof(RqSetWindowAttributes));
}

void pronDisconnect(Display *d) {
  tsock_close(d->fd);
  delete d; 
}

void pronSelectInput(Display *d, Window w, uint32_t eventMask) {
  RqSelectInput rq(w, eventMask);
  tsock_write(d->fd, &rq, sizeof(rq));
}

int pronNextEvent(Display * d, PronEvent * e) {
  /*while (tsock_read(d->fd, e, 1024) < 0) {
    usleep(100000);
  }*/
  return (tsock_read(d->fd, e, 1024) > 0);
}
  
PronEvent* getPronEvent() {
  int size = sizeof(EventWindowCreated);
  
  return (PronEvent *) malloc(size);
}

void pronReparentWindow(Display *d, Window w, Window newParent) {
  RqReparent rq(w, newParent);
  tsock_write(d->fd, &rq, sizeof(rq));
}
