#include <cstdio>
#include <clibtacos>
#include <keyboard.h>
#include <mouse.h>
#include <pron.h>
#include <tsock.h>
#include <unistd.h>
#include <vector>

using namespace std;

static Screen *screen;
static Mouse *mouse;
static Keyboard *keyboard;
static vector<Client*> clients;
int clientsFd, newClientFd;

void PronInit() {
  printf(                                             
    "                                             " "\n"
    "     _/_/_/    _/  _/_/    _/_/    _/_/_/    " "\n"
    "    _/    _/  _/_/      _/    _/  _/    _/   " "\n"
    "   _/    _/  _/        _/    _/  _/    _/    " "\n"
    "  _/_/_/    _/          _/_/    _/    _/     " "\n"
    " _/                                          " "\n"
    "_/                                           " "\n"
  );

  // Initialize screen
  screen = Screen::getInstance(800, 600, 24);
  
  // Initialize devices
  mouse = Mouse::getInstance();
  keyboard = new Keyboard();
  
  // Create root window
  screen->tree->setRoot(new Window(screen, 0, NULL, NULL, 0, 0, 800, 600));
  screen->tree->getRoot()->map();

  // Listen for clients
  unlink("/tmp/pron.sock");
  clientsFd = tsock_listen("/tmp/pron.sock");
}

void PronAcceptClient() {
  if ((newClientFd = tsock_accept(clientsFd)) > 0) {
    Client *newClient = new Client(clients.size() + 1, newClientFd);
    debug("New client (id %d, fd %d)!\n", newClient->id, newClient->fd);
    clients.push_back(newClient);
  }
}

void PronSelect() {
  // Try to accept new client
  PronAcceptClient();

  // Handle requests from clients
  //printf("clients list : ");
  for (unsigned int client = 0; client < clients.size(); client++) {
    //printf("%d ",clients[client]->fd);
    // check if client has sent a destroy request
    if (clients[client]->fd == -1) {
      delete clients[client];
      clients.erase(clients.begin() + client);
    }
    else {
      clients[client]->handle();
    }
  }
  //printf("\n");
  
  // Read from devices
  mouse->checkEvents();
  keyboard->checkEvents();

  // Restore the pointer Background
  // TODO : This fonction has to be called before everything in pronSelect() 
  // and it is the right way the right way to deal with mouse pointer graphic.
  // It is not possible now because of the asynchronisation between memory video and video buffer
  // pointer is drawn less time than computing time and so it has lower probability to be drawn
  // and in my case it is nevers drawn a the screen
  mouse->restorePointerBackground();
  // We can backup pointer background and draw it now
  mouse->drawPointer();
}

int main() {
  PronInit();

  while (1) {
    PronSelect();
  }

  return 0;
}
