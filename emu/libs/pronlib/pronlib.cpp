/**
 * @file pronlib.cpp
 * Implementation of the pronlib.
 */
#include <cstdio>
#include <pronlib.h>
#include <unistd.h>

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

void pronDisconnect(Display *d) {
  RqGoodbye rq;
  tsock_write(d->fd,&rq, sizeof(rq));
  //tsock_close(d->fd);
  //delete d; 
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

GC pronCreateGC(Display *d, const PronGCValues &values, unsigned int mask) {
  GC gc = d->newResourceId();
  RqCreateGC rq(gc, values, mask);
  tsock_write(d->fd, &rq, sizeof(rq));
  
  return gc;
}

void pronGetGCValues(Display *d, GC gc, PronGCValues *values) {
  RqGetGCValues rq(gc);
  tsock_write(d->fd, &rq, sizeof(rq));
  RespGCValues res(*values);
  d->read(RS_GC_VALUES, &res, sizeof(res));
  *values = res.values;
}

void pronChangeGC(Display *d, GC gc, const PronGCValues &values, unsigned int mask) {
  RqChangeGC rq(gc, values, mask);
  tsock_write(d->fd, &rq, sizeof(rq));
}

void pronFreeGC(Display *d, GC gc) {
  RqFreeGC rq(gc);
  tsock_write(d->fd, &rq, sizeof(rq));
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

int pronDrawPoint(Display *d, Window w, GC gc, int x, int y) {
  RqDrawPoint rq(gc, w, x, y);
  return (tsock_write(d->fd, &rq, sizeof(rq)) > 0);
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

void pronGetWindowAttributes(Display *d, Window w, PronWindowAttributes *attr) {
  RqGetWindowAttributes rq(w);
  tsock_write(d->fd, &rq, sizeof(rq)) ;
  RespWindowAttributes res(*attr);
  d->read(RS_WINDOW_ATTRIBUTES, &res, sizeof(RespWindowAttributes));
  *attr = res.attributes;
}

void pronSetWindowAttributes(Display * d, Window w, const PronWindowAttributes & newAttr, unsigned int mask) {  
  RqSetWindowAttributes rq(w,newAttr,mask);
  tsock_write(d->fd,&rq,sizeof(RqSetWindowAttributes));
}

void pronSelectInput(Display *d, Window w, uint32_t eventMask) {
  RqSelectInput rq(w, eventMask);
  tsock_write(d->fd, &rq, sizeof(rq));
}

void pronDontPropagateEvent(Display *d, Window w ,uint32_t eventMask) {
  RqDontPropagate rq(w, eventMask);
  tsock_write(d->fd, &rq, sizeof(rq));
}

int pronNextEvent(Display *d, PronEvent *e, bool nonBlocking) {
  if (nonBlocking) {
    tsock_set_nonblocking(d->fd);
  }

  int ret = d->getNextEvent(e);

  if (nonBlocking) {
    tsock_set_blocking(d->fd);
  }

  return ret;
}
  
PronEvent* getPronEvent() {
  return (PronEvent*) malloc(MAX_MSG_SIZE);
}

void pronReparentWindow(Display *d, Window w, Window newParent) {
  RqReparent rq(w, newParent);
  tsock_write(d->fd, &rq, sizeof(rq));
}

void pronDestroyWindow(Display *d, unsigned int w) {
  RqDestroyWindow rq(w);
  tsock_write(d->fd, &rq, sizeof(rq));
} 

void pronMoveWindow(Display *d, unsigned int w, int x, int y) {
  RqMoveWindow rq(w, x, y);
  tsock_write(d->fd, &rq, sizeof(rq));
}

void pronPutImage(Display *d, Window w, GC gc, PronImage *image, 
    int srcX, int srcY, int destX, int destY, int width, int height) {
  // We have to check if the rectangle the client wants to send enters 
  // in the source image
  if((srcX + width > image->width) || (srcY + height > image->height)) {
    fprintf(stderr, "Subimage is too large\n");
    return;
  }
  // Compute the size of the needed send buffer
  int bufferSize = sizeof(RqPutImage) + width * height * image->depth;
  // Test if we can send the image 
  if(! (bufferSize <= MAX_MSG_SIZE)) {
    fprintf(stderr, "Message is to small\n");
    return;
  }
  // We allocate the image request buffer
  char *buf = (char *)malloc(bufferSize);
  // Creation of the request object
  RqPutImage rq(w, width, height, destX, destY, image->depth, image->format);
  // Copy of the request object in the send buffer
  memcpy(buf, &rq, sizeof(rq));
  // Now we have to copy the subimage we have to send
  for(int y = 0; y < height; y++){
    for(int x = 0; x < width; x++){
      // Buffer destination
      void * dest = buf + sizeof(rq) + (x + y * width) * image->depth ;
      // Image source
      void * src = image->data + (srcY * image->width + srcX + x + y * width) * image->depth;
      // Copy the pixel dude
      memcpy(dest, src, image->depth);
    }
  }
  // We can send the buffer
  tsock_write(d->fd, buf, bufferSize);
  // free the buffer
  free(buf);
  // That's all folks
}

void pronResizeWindow(Display *d, unsigned int w, int width, int height) {
  RqResizeWindow rq(w, width, height);
  tsock_write(d->fd, &rq, sizeof(rq));
}
