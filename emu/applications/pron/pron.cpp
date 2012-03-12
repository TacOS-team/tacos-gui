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
  mouse = new Mouse();
  keyboard = new Keyboard();
  
  // Create root window
  screen->root = new Window(screen, 0, NULL, NULL, 0, 0, 800, 600);
  screen->addWindow(screen->root);

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
  for (unsigned int client = 0; client < clients.size(); client++) {
    clients[client]->handle();
  }
  
  // Read from devices
  mouse->checkEvents();
  keyboard->checkEvents();
}

int main() {
  PronInit();

  while (1) {
    PronSelect();
  }

  return 0;
}
