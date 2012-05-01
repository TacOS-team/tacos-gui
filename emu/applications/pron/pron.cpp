/**
 * @file pron.cpp
 * Pron main file.
 */ 

#include <stdio.h>
#include <string>
#include <unistd.h>
#include <vector>

#include <client.h>
#include <keyboard.h>
#include <mouse.h>
#include <screen.h>
#include <window.h>

#include <libtacos.h>
#include <tsock.h>

using namespace std;

static Screen *screen; /**< Screen instance */
static Mouse *mouse; /**< Mouse instance  */
static Keyboard *keyboard; /**< Keyboard instance */
static vector<Client*> clients; /**< List of currently connected clients */
int clientsFd; /**< File descriptor of the socket listening for new clients */
unsigned int newClientID; /**< Id given to the last client who has connected to pron */

/**
 * Initialisation function.
 * Creates the screen and the root window, initializes devices
 * (keyboard and mouse) and listens for clients.
 */
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

  // The id of the first client to connect will be 0
  newClientID = 0;

  // Listen for clients
  unlink("/tmp/pron.sock");
  clientsFd = tsock_listen("/tmp/pron.sock");
  tsock_set_nonblocking(clientsFd);

  string welcome = "Welcome to pron!";
  screen->getRoot()->drawText(10, 20, welcome.c_str(), welcome.length());
}

/**
 * Tries to accept a new client.
 */
void PronAcceptClient() {
  int newClientFd;
  if ((newClientFd = tsock_accept(clientsFd)) > 0) {
    tsock_set_nonblocking(newClientFd);
    Client *newClient = new Client(++newClientID, newClientFd);
    debug("New client (id %d, fd %d)!\n", newClient->id, newClient->fd);
    clients.push_back(newClient);
  }
}

/**
 * Main iteration: reads all file descriptors to accept new clients,
 * handle existing clients and read from devices (keyboard and mouse).
 */
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

  /**
   * Restore the pointer Background
   * @todo This fonction has to be called before everything in pronSelect() 
   * and it is the right way the right way to deal with mouse pointer graphic.
   * It is not possible now because of the asynchronisation between memory video and video buffer
   * pointer is drawn less time than computing time and so it has lower probability to be drawn
   * and in my case it is nevers drawn a the screen
   */
  mouse->restorePointerBackground();
  // We can backup pointer background and draw it now
  mouse->drawPointer();
}

/**
 * Pron main function.
 */
int main() {
  PronInit();

  while (1) {
    PronSelect();
  }

  return 0;
}
