int open(const char *pathname, int flags) {
  int fd;

  if (strcmp(pathname, "/dev/vga") == 0) {
    // VGA driver
    fd = libc_open("/tmp", 0); // dummy fd
    tacos_descriptors[fd] = VGA;
    if (!SDL_initialized) {
      init_sdl();
    }
  } else if (strcmp(pathname, "/dev/vesa") == 0) {
    // VESA driver
    fd = libc_open("/tmp", 0); // dummy fd
    tacos_descriptors[fd] = VESA;
    if (!SDL_initialized) {
      init_sdl();
    }
  } else if (strcmp(pathname, "/dev/mouse") == 0) {
    // Mouse driver
    fd = libc_open("/tmp", 0); // dummy fd
    tacos_descriptors[fd] = MOUSE;
  } else if (strcmp(pathname, "/dev/keyboard") == 0) {
    // Keyboard driver
    fd = libc_open("/tmp", 0); // dummy fd
    tacos_descriptors[fd] = KEYBOARD;
  } else {
    // Native open
    fd = libc_open(pathname, flags);
  }
  
  return fd;
}
