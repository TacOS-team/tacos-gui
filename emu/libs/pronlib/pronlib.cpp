/**
 * @file pronlib.cpp
 * Implementation of the pronlib.
 */
#include <stdio.h>
#include <unistd.h>

#include <pronlib.h>

/**
 * Pron namespace
 */
namespace pron {

Display::Display(int fd, RespWelcome *welcome) {
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

int Display::newResourceId() {
  return this->curId++;
}

int Display::read(PronMessage *msg, size_t len) {
  int sizeRead = tsock_read(this->fd, msg, len);

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

int Display::read(MessageType type, void *buffer, size_t len) {
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

bool Display::getNextEvent(PronEvent *e) {
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

void pronMoveWindowTo(Display *d, unsigned int w, int x, int y) {
  RqMoveWindowTo rq(w, x, y);
  tsock_write(d->fd, &rq, sizeof(rq));
}

void pronPutImage(Display *d, Window w, GC __attribute__((unused)) gc, PronImage *image,
    int srcX, int srcY, int width, int height, int destX, int destY) {
  // TODO: use GC?
  // We have to check if the rectangle the client wants to send enters 
  // in the source image
  if((srcX + width > image->width) || (srcY + height > image->height)) {
    fprintf(stderr, "Subimage is too large\n");
    return;
  }
  // Compute the size of the needed send buffer
  int bufferSize = sizeof(RqPutImage) + width * height * image->bytesPerPixel;
  // Test if we can send the image 
  if(! (bufferSize <= MAX_MSG_SIZE)) {
    fprintf(stderr, "Message is to small\n");
    return;
  }
  // We allocate the image request buffer
  char *buf = (char*) malloc(bufferSize);
  // Creation of the request object
  RqPutImage rq(w, destX, destY, width, height, image->format, image->depth, image->bytesPerPixel);
  // Copy of the request object in the send buffer
  memcpy(buf, &rq, sizeof(rq));
  // Now we have to copy the subimage we have to send
  for(int y = 0; y < height; y++){
    for(int x = 0; x < width; x++){
      // Buffer destination
      void * dest = buf + sizeof(rq) + (x + y * width) * image->bytesPerPixel ;
      // Image source
      void * src = image->data + (srcY * image->width + srcX + x + y * image->width) * image->bytesPerPixel;
      // Copy the pixel dude
      //printf("Pixel colors {%d, %d, %d}\n", *((char*) dest) & ~(0xFFFFFF00), (*((char*) dest) >> 8 ) & ~(0xFFFFFF00), (*((char*) dest) >> 16) & ~(0xFFFFFF00));
      memcpy(dest, src, image->bytesPerPixel);
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

Pixmap pronCreatePixmap(Display *d, int width, int height, int depth) {
  if(depth == 24/* || Other available depths*/) {
    Pixmap p = (Pixmap) d->newResourceId();
    RqCreatePixmap rq(p, width, height, depth);
    tsock_write(d->fd, &rq, sizeof(rq));

    return p;
  }
  return -1;
}

void pronFreePixmap(Display *d, unsigned int p) {
  RqFreePixmap rq(p);
  tsock_write(d->fd, &rq, sizeof(rq));
}

void pronCopyArea(Display *d, Drawable src, Drawable dest, GC gc, 
    int srcX, int srcY, unsigned int width, 
    unsigned int height, int destX, int destY) {
  RqCopyArea rq(src, dest, gc, srcX, srcY, width, height, destX, destY);
  tsock_write(d->fd, &rq, sizeof(rq));
}

void pronDrawText(Display *d, Window w, GC gc, int x, int y,
    const char *text, int length) {
  RqDrawText rq(gc, w, x, y, text, length);
  tsock_write(d->fd, &rq, sizeof(rq));
}

} // namespace pron
