#include <stdio.h>

#include <client.h>
#include <drawable.h>
#include <pixmap.h>
#include <pron_proto.h>
#include <screen.h>
#include <tsock.h>
#include <window.h>

int Client::recvLen;
char Client::recvBuf[MAX_MSG_SIZE];

Client::Client(int id, int fd) {
  this->id = id;
  this->fd = fd;
}

void Client::handle() {
  if ((Client::recvLen = tsock_read(this->fd, Client::recvBuf, sizeof(Client::recvBuf))) <= 0) {
    return;
  }

  int reqType = *((int*) Client::recvBuf);
  Screen *screen = Screen::getInstance();

  // TODO: refactor when we have exceptions? :)
  switch (reqType) { 
    case RQ_HELLO: {
      // Identifiers: 16 upper bits for client id, 16 lower bits for resource id
      RespWelcome welcome(screen->tree->getRoot()->getId(), this->id << 16, ((this->id + 1) << 16) - 1);
      this->send(&welcome, sizeof(welcome));
      break;
    }
    case RQ_GOODBYE: {
      printf("RQ_GOODBYE from fd %d\n", this->fd);
      tsock_close(this->fd);
      this->fd = -1; // so that pron knows he can delete the client
      break;
    }
    case RQ_CREATE_WINDOW: {
      RqCreateWindow *rq = (RqCreateWindow*) Client::recvBuf;
      new Window(screen, rq->id, this, (Window*) screen->getDrawable(rq->parent, D_WINDOW),
        rq->x, rq->y, rq->width, rq->height);
      screen->traceWindows();
      break;
    }
    case RQ_MAP_WINDOW: {
      RqMapWindow *rq = (RqMapWindow*) Client::recvBuf;
      Window *w = (Window*) screen->getDrawable(rq->window, D_WINDOW);
      if (w != NULL) {
        w->map();
      }
      break;
    }
    case RQ_UNMAP_WINDOW: {
      RqUnmapWindow *rq = (RqUnmapWindow*) Client::recvBuf;
      Window *w = (Window*) screen->getDrawable(rq->window, D_WINDOW);
      if (w != NULL) {
        w->unmap();
      }
      break;
    }
    case RQ_RAISE_WINDOW: {
      RqRaiseWindow *rq = (RqRaiseWindow*) Client::recvBuf;
      Window *w = (Window*) screen->getDrawable(rq->window, D_WINDOW);
      if (w != NULL) {
        w->raise();
      }
      break;
    }
    case RQ_CREATE_GC: {
      RqCreateGC *rq = (RqCreateGC*) Client::recvBuf;
      new GC(rq->id, rq->values, rq->mask);
      break;
    }
    case RQ_GET_GC_VALUES: {
      RqGetGCValues *rq = (RqGetGCValues*) Client::recvBuf;
      GC *gc = GC::getGC(rq->gc);
      if (gc != NULL) {
        RespGCValues resp (gc->getValues());
        this->send(&resp, sizeof(resp));
      }
      break;
    }
    case RQ_CHANGE_GC: {
      RqChangeGC *rq = (RqChangeGC*) Client::recvBuf;
      GC *gc = GC::getGC(rq->gc);
      if (gc != NULL) {
        gc->setValues(rq->values, rq->mask);
      }
      break;
    }
    case RQ_FREE_GC: {
      RqFreeGC *rq = (RqFreeGC*) Client::recvBuf;
      GC *gc = GC::getGC(rq->gc);
      if (gc != NULL) {
        gc->destroy();
      }
      break;
    }
    case RQ_SELECT_INPUT: {
      RqSelectInput *rq = (RqSelectInput*) Client::recvBuf;
      Window *w = (Window*) screen->getDrawable(rq->window, D_WINDOW);
      if (w != NULL) {
        w->selectInput(this, rq->eventMask);
      }
      break;
    }
    case RQ_DONT_PROPAGATE: {
      RqDontPropagate *rq = (RqDontPropagate*) Client::recvBuf;
      Window *w = (Window*) screen->getDrawable(rq->window, D_WINDOW);
      if (w != NULL) {
        w->dontPropagateMask = rq->eventMask;  
      }
      break;
    }
    case RQ_GET_WINDOW_ATTRIBUTES: {
      RqGetWindowAttributes *rq = (RqGetWindowAttributes*) Client::recvBuf;
      Window *w = (Window*) screen->getDrawable(rq->window, D_WINDOW);
      if (w != NULL) {
        RespWindowAttributes resp (w->getAttributes());
        this->send(&resp, sizeof(resp));
      }
      break;
    }
    case RQ_SET_WINDOW_ATTRIBUTES: {
      RqSetWindowAttributes *rq = (RqSetWindowAttributes*) Client::recvBuf;
      Window *w = (Window*) screen->getDrawable(rq->window, D_WINDOW);
      if (w != NULL) {
        w->setAttributes(&(rq->newAttr), rq->mask);
      }
      break;
    }
    case RQ_CLEAR_WINDOW: {
      RqClearWindow *rq = (RqClearWindow*) Client::recvBuf;
      Window *w = (Window*) screen->getDrawable(rq->window);
      if (w != NULL && screen->prepareDrawing(w)) {
        w->clear();
      }
      break;
    }
    case RQ_DRAW_POINT: {
      RqDrawPoint *rq = (RqDrawPoint*) Client::recvBuf;
      Drawable *d = screen->getDrawable(rq->drawable);
      GC *gc = GC::getGC(rq->gc);
      if (d != NULL && screen->prepareDrawing(d, gc)) {
        d->drawPoint(rq->x, rq->y);
      }
      break;
    }
    case RQ_DRAW_CIRCLE: {
      RqDrawCircle *rq = (RqDrawCircle*) Client::recvBuf;
      Drawable *d = screen->getDrawable(rq->drawable);
      GC *gc = GC::getGC(rq->gc);
      if (d != NULL && screen->prepareDrawing(d, gc)) {
        d->drawCircle(rq->x, rq->y, rq->radius);
      }
      break;
    }
    case RQ_DRAW_LINE: {
      RqDrawLine *rq = (RqDrawLine*) Client::recvBuf;
      Drawable *d = screen->getDrawable(rq->drawable);
      GC *gc = GC::getGC(rq->gc);
      if (d != NULL && screen->prepareDrawing(d, gc)) {
        d->drawLine(rq->x1, rq->y1, rq->x2, rq->y2);
      }
      break;
    }
    case RQ_DRAW_RECT: {
      RqDrawRect *rq = (RqDrawRect*) Client::recvBuf;
      Drawable *d = screen->getDrawable(rq->drawable);
      GC *gc = GC::getGC(rq->gc);
      if (d != NULL && screen->prepareDrawing(d, gc)) {
        d->drawRect(rq->x, rq->y, rq->width, rq->height);
      }
      break;
    }
    case RQ_FILL_CIRCLE: {
      RqFillCircle *rq = (RqFillCircle*) Client::recvBuf;
      Drawable *d = screen->getDrawable(rq->drawable);
      GC *gc = GC::getGC(rq->gc);
      if (d != NULL && screen->prepareDrawing(d, gc)) {
        d->fillCircle(rq->x, rq->y, rq->radius);
      }
      break;
    }
    case RQ_FILL_RECTANGLE: {
      RqFillRectangle *rq = (RqFillRectangle*) Client::recvBuf;
      Drawable *d = screen->getDrawable(rq->drawable);
      GC *gc = GC::getGC(rq->gc);
      if (d != NULL && screen->prepareDrawing(d, gc)) {
        d->fillRectangle(rq->x, rq->y, rq->width, rq->height);
      }
      break;
    }
    case RQ_DRAW_TEXT: {
      RqDrawText *rq = (RqDrawText*) Client::recvBuf;
      Window *w = (Window*) screen->getDrawable(rq->window, D_WINDOW);
      GC *gc = GC::getGC(rq->gc);
      if (w != NULL && screen->prepareDrawing(w, gc)) {
        w->drawText(rq->x, rq->y, rq->text, rq->length);
      }
      break;
    }
    case RQ_REPARENT: {
      RqReparent *rq = (RqReparent*) Client::recvBuf;
      Window *child = (Window*) screen->getDrawable(rq->window, D_WINDOW);
      Window *newParent = (Window*) screen->getDrawable(rq->newParent, D_WINDOW);
      if (child != NULL && newParent != NULL) {
        screen->reparent(child, newParent);
      }
      break;
    }
    case RQ_DESTROY_WINDOW: {
      RqDestroyWindow *rq = (RqDestroyWindow*) Client::recvBuf;
      Window *w = (Window*) screen->getDrawable(rq->window, D_WINDOW);
      if (w != NULL) {
        w->destroy();  
      }
      screen->traceWindows();
      break;
    }
    case RQ_MOVE_WINDOW: {
      RqMoveWindow *rq = (RqMoveWindow*) Client::recvBuf;
      Window *w = (Window*) screen->getDrawable(rq->window, D_WINDOW);
      if (w != NULL) {
        w->move(rq->x, rq->y);
      }
      break;
    }
    case RQ_MOVE_WINDOW_TO: { // TODO Faire une fonction générale pour les deux move
      RqMoveWindowTo *rq = (RqMoveWindowTo*) Client::recvBuf;
      Window *w = (Window*) screen->getDrawable(rq->window, D_WINDOW);
      if (w != NULL) {
        w->moveTo(rq->x, rq->y);
      }
      break;
    }
    case RQ_PUT_IMAGE: {
      RqPutImage *rq = (RqPutImage*) Client::recvBuf;
      Window *w = (Window*) screen->getDrawable(rq->window);
      if (w != NULL) {
      	char *image_buf = ((char*) rq) + sizeof(RqPutImage);
        PronImage image(rq->width, rq->height, rq->format, image_buf, rq->depth, rq->bytesPerPixel, false);
        w->putImage(&image, rq->x, rq->y);
      }
      break;
    }
    case RQ_RESIZE_WINDOW: {
      RqResizeWindow *rq = (RqResizeWindow*) Client::recvBuf;
      Window *w = (Window*) screen->getDrawable(rq->window, D_WINDOW);
      if (w != NULL) {
        w->resize(rq->width, rq->height);
      }
      break;
    }
    case RQ_CREATE_PIXMAP: {
      RqCreatePixmap *rq = (RqCreatePixmap*) Client::recvBuf;
      new Pixmap(screen, rq->id, this, rq->width, rq->height, rq->depth);
      break;
    }
    case RQ_FREE_PIXMAP: {
      RqFreePixmap *rq = (RqFreePixmap*) Client::recvBuf;
      Pixmap *p = (Pixmap*) screen->getDrawable(rq->pixmap, D_PIXMAP);
      delete p;
      break;
    }
    case RQ_COPY_AREA: {
      RqCopyArea *rq = (RqCopyArea*) Client::recvBuf;
      Pixmap *p = (Pixmap*) screen->getDrawable(rq->src, D_PIXMAP);
      Window *w = (Window*) screen->getDrawable(rq->dest, D_WINDOW);
      GC *gc = GC::getGC(rq->gc);
      if (p != NULL && w != NULL && screen->prepareDrawing(w, gc)) {
        w->copyArea(rq->destX, rq->destY, p, rq->srcX, rq->srcY, rq->width, rq->height);
      }
      break;
    }
  }
}

void Client::send(void *data, unsigned int size) {
  tsock_write(this->fd, data, size);
}
