#include <client.h>
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

  switch (reqType) { 
    case RQ_HELLO: {
      // Identifiers: 16 upper bits for client id, 16 lower bits for resource id
      RespWelcome welcome(screen->root->id, this->id << 16, (this->id << 17) - 1);
      this->send(&welcome, sizeof(welcome));
      break;
    }
    case RQ_CREATE_WINDOW: {
      RqCreateWindow *rq = (RqCreateWindow*) Client::recvBuf;
      Window *w = new Window(screen, rq->id, this, screen->getWindow(rq->parent), rq->x, rq->y, rq->width, rq->height);
      screen->addWindow(w);
      EventWindowCreated eventCreated(w->id, rq->parent, w->getAttributes());
      w->deliverWindowEvent(&eventCreated, sizeof(eventCreated));
      screen->traceWindows();
      break;
    }
    case RQ_MAP_WINDOW: {
      RqMapWindow *rq = (RqMapWindow*) Client::recvBuf;
      screen->getWindow(rq->window)->map();
      break;
    }
    case RQ_UNMAP_WINDOW: {
      RqUnmapWindow *rq = (RqUnmapWindow*) Client::recvBuf;
      screen->getWindow(rq->window)->unmap();
      break;
    }
    case RQ_RAISE_WINDOW: {
      RqRaiseWindow *rq = (RqRaiseWindow*) Client::recvBuf;
      screen->getWindow(rq->window)->raise();
      break;
    }
    case RQ_CREATE_GC: {
      break;
    }
    case RQ_SELECT_INPUT: {
      RqSelectInput *rq = (RqSelectInput*) Client::recvBuf;
      screen->getWindow(rq->window)->selectInput(this, rq->eventMask);
      break;
    }
    case RQ_GET_WINDOW_ATTRIBUTES: {
      RqGetWindowAttributes *rq = (RqGetWindowAttributes*) Client::recvBuf;
      RespWindowAttributes resp (screen->getWindow(rq->w)->getAttributes());
      this->send(&resp, sizeof(resp)); 
      break;
    }
    case RQ_SET_WINDOW_ATTRIBUTES: {
      RqSetWindowAttributes *rq = (RqSetWindowAttributes*) Client::recvBuf;
      screen->getWindow(rq->w)->setAttributes(&(rq->newAttr), rq->mask);
      break;
    }
    case RQ_CLEAR_WINDOW: {
      RqClearWindow *rq = (RqClearWindow*) Client::recvBuf;
      Window *w = screen->getWindow(rq->window);
      if (screen->prepareDrawing(w)) {
        w->clear();
      }
      break;
    }
    case RQ_DRAW_CIRCLE: {
      RqDrawCircle *rq = (RqDrawCircle*) Client::recvBuf;
      Window *w = screen->getWindow(rq->drawable);
      if (screen->prepareDrawing(w)) {
        w->drawCircle(rq->x, rq->y, rq->radius);
      }
      break;
    }
    case RQ_DRAW_LINE: {
      RqDrawLine *rq = (RqDrawLine*) Client::recvBuf;
      Window *w = screen->getWindow(rq->drawable);
      if (screen->prepareDrawing(w)) {
        w->drawLine(rq->x1, rq->y1, rq->x2, rq->y2);
      }
      break;
    }
    case RQ_DRAW_RECT: {
      RqDrawRect *rq = (RqDrawRect*) Client::recvBuf;
      Window *w = screen->getWindow(rq->drawable);
      if (screen->prepareDrawing(w)) {
        w->drawRect(rq->x, rq->y, rq->width, rq->height);
      }
      break;
    }
    case RQ_FILL_CIRCLE: {
      RqFillCircle *rq = (RqFillCircle*) Client::recvBuf;
      Window *w = screen->getWindow(rq->drawable);
      if (screen->prepareDrawing(w)) {
        w->fillCircle(rq->x, rq->y, rq->radius);
      }
      break;
    }
    case RQ_FILL_RECTANGLE: {
      RqFillRectangle *rq = (RqFillRectangle*) Client::recvBuf;
      Window *w = screen->getWindow(rq->drawable);
      if (screen->prepareDrawing(w)) {
        w->fillRectangle(rq->x, rq->y, rq->width, rq->height);
      }
      break;
    }
    case RQ_REPARENT: {
      RqReparent *rq = (RqReparent*) Client::recvBuf;
      Window *w = screen->getWindow(rq->window);
      w->reparent(screen->getWindow(rq->newParent));
      screen->clipWin = NULL;
      screen->traceWindows();
      break;
    }
  }
}

void Client::send(void *data, unsigned int size) {
  tsock_write(this->fd, data, size);
}
