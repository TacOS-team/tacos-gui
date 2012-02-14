#include <cstdio>
#include <pron.h>
#include <tsock.h>
#include <unistd.h>
#include <signal.h>
#include <sys/time.h>

#define MAX_CLIENTS 128
#define MAX_MSG_SIZE 1024

static Screen *screen;
static Client clients[MAX_CLIENTS+1];
static int nbClients = 0;

static void alarm_handler(int signum) {
  screen->flush();
}

void handleClientRequest(int client, void *buf, int size) {
  int reqType = *((int*)buf);
  switch (reqType) {
    case RQ_HELLO: {
      //printf("Received RQ_HELLO from client %d!\n", client);
      //RqHello *rq = (RqHello*)buf;
      // Identifiers: 16 upper bits for client id, 16 lower bits for resource id
      RespWelcome welcome(screen->root->id, client << 16, (client << 17) - 1);
      tsock_write(clients[client].fd, &welcome, sizeof(welcome));
      break;
    }
    case RQ_CREATE_WINDOW: {
      //printf("Received RQ_CREATE_WINDOW from client %d!\n", client);
      RqCreateWindow *rq = (RqCreateWindow*) buf;
      Window *w = new Window(screen, rq->id, screen->getWindow(rq->parent), rq->x, rq->y, rq->width, rq->height);
      screen->addWindow(w);
      break;
    }
    case RQ_CLEAR_WINDOW: {
      //printf("Received RQ_CLEAR_WINDOW from client %d!\n", client);
      RqClearWindow *rq = (RqClearWindow*) buf;
      screen->getWindow(rq->window)->clear();
      break;
    }
    case RQ_MAP_WINDOW: {
      //printf("Received RQ_MAP_WINDOW from client %d!\n", client);
      break;
    }
    case RQ_CREATE_GC: {
      //printf("Received RQ_CREATE_GC from client %d!\n", client);
      break;
    }
    case RQ_DRAW_LINE: {
      //printf("Received RQ_DRAW_LINE from client %d!\n", client);
      RqDrawLine *rq = (RqDrawLine*) buf;
      screen->getWindow(rq->drawable)->drawLine(rq->x1, rq->y1, rq->x2, rq->y2);
      break;
    }
    case RQ_FILL_RECTANGLE: {
      //printf("Received RQ_FILL_RECTANGLE from client %d!\n", client);
      RqFillRectangle *rq = (RqFillRectangle*) buf;
      screen->getWindow(rq->drawable)->fillRectangle(rq->x1, rq->y1, rq->x2, rq->y2);
      break;
    }
    case RQ_SELECT_INPUT: {
      //printf("Received RQ_SELECT_INPUT from client %d!\n", client);
      RqSelectInput *rq = (RqSelectInput*) buf;
      screen->getWindow(rq->window)->selectInput(client, rq->eventMask);
    }
  }
}

int main() {
  printf("Welcome to pr0n \\o/\n");

  // Initialize screen
  screen = new Screen(800, 600, 24);
  // Create root window
  screen->root = new Window(screen, 0, NULL, 0, 0, 800, 600);
  screen->addWindow(screen->root);

  // Set up the flush timer
  signal(SIGALRM, alarm_handler);
  struct itimerval flushTimer;
  flushTimer.it_interval.tv_usec = 40000;
  flushTimer.it_interval.tv_sec = 0;
  flushTimer.it_value.tv_usec = 40000;
  flushTimer.it_value.tv_sec = 0;
  setitimer(ITIMER_REAL, &flushTimer, 0);
  
  // Listen for clients
  unlink("/tmp/pron.sock");
  int fd = tsock_listen("/tmp/pron.sock");

  // Main loop
  bool continuer = true;
  int newFd, lRead;
  char buf[MAX_MSG_SIZE];
  while (continuer) {
    // Try to accept new client
    if ((newFd = tsock_accept(fd)) > 0) {
      printf("New client (%d)!\n", nbClients);
      nbClients++;
      clients[nbClients].fd = newFd;
    }

    // Read requests from clients
    for (int client = 1; client <= nbClients; client++) {
      if ((lRead = tsock_read(clients[client].fd, buf, sizeof(buf))) > 0) {
        handleClientRequest(client, buf, lRead);
      }
    }
  }

  // Close the listening socket
  tsock_close(fd);
}
