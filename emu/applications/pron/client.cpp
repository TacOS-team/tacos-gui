#include <client.h>
#include <pron_proto.h>
#include <screen.h>
#include <tsock.h>
#include <window.h>
#include <cstdio>

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
      RespWelcome welcome(screen->root->id, this->id << 16, (this->id << 17) - 1);
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
      new Window(screen, rq->id, this, screen->getWindow(rq->parent), rq->x, rq->y, rq->width, rq->height);
      //screen->traceWindows();
      break;
    }
    case RQ_MAP_WINDOW: {
      RqMapWindow *rq = (RqMapWindow*) Client::recvBuf;
      Window *w = screen->getWindow(rq->window);
      if (w != NULL) {
        w->map();
      }
      break;
    }
    case RQ_UNMAP_WINDOW: {
      RqUnmapWindow *rq = (RqUnmapWindow*) Client::recvBuf;
      Window *w = screen->getWindow(rq->window);
      if (w != NULL) {
        w->unmap();
      }
      break;
    }
    case RQ_RAISE_WINDOW: {
      RqRaiseWindow *rq = (RqRaiseWindow*) Client::recvBuf;
      Window *w = screen->getWindow(rq->window);
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
      Window *w = screen->getWindow(rq->window);
      if (w != NULL) {
        w->selectInput(this, rq->eventMask);
      }
      break;
    }
    case RQ_DONT_PROPAGATE: {
      RqDontPropagate *rq = (RqDontPropagate*) Client::recvBuf;
      Window *w = screen->getWindow(rq->window);
      if (w != NULL) {
        w->dontPropagateMask = rq->eventMask;  
      }
      break;
    }
    case RQ_GET_WINDOW_ATTRIBUTES: {
      RqGetWindowAttributes *rq = (RqGetWindowAttributes*) Client::recvBuf;
      Window *w = screen->getWindow(rq->window);
      if (w != NULL) {
        RespWindowAttributes resp (w->getAttributes());
        this->send(&resp, sizeof(resp));
      }
      break;
    }
    case RQ_SET_WINDOW_ATTRIBUTES: {
      RqSetWindowAttributes *rq = (RqSetWindowAttributes*) Client::recvBuf;
      Window *w = screen->getWindow(rq->window);
      if (w != NULL) {
        w->setAttributes(&(rq->newAttr), rq->mask);
      }
      break;
    }
    case RQ_CLEAR_WINDOW: {
      RqClearWindow *rq = (RqClearWindow*) Client::recvBuf;
      Window *w = screen->getWindow(rq->window);
      if (w != NULL && screen->prepareDrawing(w)) {
        w->clear();
      }
      break;
    }
    case RQ_DRAW_POINT: {
      RqDrawPoint *rq = (RqDrawPoint*) Client::recvBuf;
      Window *w = screen->getWindow(rq->drawable);
      GC *gc = GC::getGC(rq->gc);
      if (w != NULL && screen->prepareDrawing(w, gc)) {
        w->drawPoint(rq->x, rq->y);
      }
      break;
    }
    case RQ_DRAW_CIRCLE: {
      RqDrawCircle *rq = (RqDrawCircle*) Client::recvBuf;
      Window *w = screen->getWindow(rq->drawable);
      GC *gc = GC::getGC(rq->gc);
      if (w != NULL && screen->prepareDrawing(w, gc)) {
        w->drawCircle(rq->x, rq->y, rq->radius);
      }
      break;
    }
    case RQ_DRAW_LINE: {
      RqDrawLine *rq = (RqDrawLine*) Client::recvBuf;
      Window *w = screen->getWindow(rq->drawable);
      GC *gc = GC::getGC(rq->gc);
      if (w != NULL && screen->prepareDrawing(w, gc)) {
        w->drawLine(rq->x1, rq->y1, rq->x2, rq->y2);
      }
      break;
    }
    case RQ_DRAW_RECT: {
      RqDrawRect *rq = (RqDrawRect*) Client::recvBuf;
      Window *w = screen->getWindow(rq->drawable);
      GC *gc = GC::getGC(rq->gc);
      if (w != NULL && screen->prepareDrawing(w, gc)) {
        w->drawRect(rq->x, rq->y, rq->width, rq->height);
      }
      break;
    }
    case RQ_FILL_CIRCLE: {
      RqFillCircle *rq = (RqFillCircle*) Client::recvBuf;
      Window *w = screen->getWindow(rq->drawable);
      GC *gc = GC::getGC(rq->gc);
      if (w != NULL && screen->prepareDrawing(w, gc)) {
        w->fillCircle(rq->x, rq->y, rq->radius);
      }
      break;
    }
    case RQ_FILL_RECTANGLE: {
      RqFillRectangle *rq = (RqFillRectangle*) Client::recvBuf;
      Window *w = screen->getWindow(rq->drawable);
      GC *gc = GC::getGC(rq->gc);
      if (w != NULL && screen->prepareDrawing(w, gc)) {
        w->fillRectangle(rq->x, rq->y, rq->width, rq->height);
      }
      break;
    }
    case RQ_REPARENT: {
      RqReparent *rq = (RqReparent*) Client::recvBuf;
      Window *w = screen->getWindow(rq->window);
      if (w != NULL) {
        w->reparent(screen->getWindow(rq->newParent));
        screen->clipWin = NULL;
        //screen->traceWindows();
      }
      break;
    }
    case RQ_DESTROY_WINDOW: {
      RqDestroyWindow *rq = (RqDestroyWindow*) Client::recvBuf;
      Window *w = screen->getWindow(rq->window);
      if (w != NULL) {
        Window *currentWindow = w;
        while (currentWindow != NULL && currentWindow != w->parent) {
          EventDestroyWindow eventDestroyWindow(currentWindow->id);
          currentWindow->deliverWindowEvent(&eventDestroyWindow, sizeof(eventDestroyWindow));
          if (currentWindow->firstChild != NULL) {
            currentWindow = currentWindow->firstChild;
          } else if (currentWindow->nextSibling != NULL) {
            currentWindow = currentWindow->nextSibling;
          } else {
            while (currentWindow->parent != NULL
                    && currentWindow->parent->nextSibling == NULL
                    && currentWindow != w->parent) {
              currentWindow = currentWindow->parent;
            }
            if (currentWindow->parent != NULL) {
              currentWindow = currentWindow->parent->nextSibling;
            }
          }
        }
        w->destroy();  
      }
      break;
    }
    case RQ_MOVE_WINDOW: {
      RqMoveWindow *rq = (RqMoveWindow*) Client::recvBuf;
      Window *w = screen->getWindow(rq->window);
      if (w != NULL) {
        // TODO Gestion sous-filles + clean
        // TODO Create method Window::move
        w->unmap();
        w->x += rq->x;
        w->y += rq->y;
        for (Window *currentChild = w->firstChild; currentChild != NULL; currentChild = currentChild->nextSibling) {
          currentChild->x += rq->x;
          currentChild->y += rq->y;
        }
        w->map();
      }
      break;
    }
    case RQ_PUT_IMAGE: {
      RqPutImage *rq = (RqPutImage*) Client::recvBuf;
      // Gets the image buffer
      char *image_buf = ((char*) rq) + sizeof(RqPutImage);
      // Gets the window 
      Window *w = screen->getWindow(rq->window);
      if(w != NULL){
        // Create the PronImage
        PronImage image(rq->width, rq->height, rq->format, image_buf, rq->depth, rq->bytesPerPixel, false);
        // Request the copy into the window
        w->putImage(&image, rq->x, rq->y);
      }
      break;
    }
    case RQ_RESIZE_WINDOW: {
      RqResizeWindow *rq = (RqResizeWindow*) Client::recvBuf;
      Window *w = screen->getWindow(rq->window);
      EventResizeWindow eventResizeWindow(rq->width, rq->height);
      w->deliverWindowEvent(&eventResizeWindow, sizeof(eventResizeWindow));
      if (w != NULL) {
        w->unmap();
        w->width  = rq->width;
        w->height = rq->height;
        w->map();
      }
      break;
    }
  }
}

void Client::send(void *data, unsigned int size) {
  tsock_write(this->fd, data, size);
}
