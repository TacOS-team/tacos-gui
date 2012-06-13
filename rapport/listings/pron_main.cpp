/**
 * Main iteration: reads all file descriptors to accept new clients,
 * handle existing clients and read from devices (keyboard and mouse).
 */
void PronSelect() {
  // Try to accept new client
  PronAcceptClient();

  // Handle requests from clients
  for (unsigned int client = 0; client < Client::clients.size(); client++) {
    Client::clients[client]->handle();
  }
  
  // Read from devices
  mouse->checkEvents();
  keyboard->checkEvents();
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
