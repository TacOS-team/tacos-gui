#define _GNU_SOURCE
#include <dlfcn.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <SDL/SDL.h>
#include <SDL/SDL_mutex.h>

// Drivers
#include <vesadrv.h>
#include <vgadrv.h>
#include <mousedrv.h>
#include <kbddrv.h>

#include <elf.h>

#define MAX_FD 128
#define MAX_SDL_EVENTS 128

SDL_Surface *screen;
SDL_mutex* mutex;

int SDL_initialized = 0;

int (*libc_open)(const char *pathname, int flags);
int (*libc_close)(int fd);
ssize_t (*libc_read)(int fd, void *buf, size_t count);
int (*libc_ioctl)(int fd, unsigned long request, void *data);

enum tacos_descriptor_type {
  NONE,
  VGA,
  VESA,
  MOUSE,
  KEYBOARD,
};

enum tacos_descriptor_type tacos_descriptors[MAX_FD];

Uint32 libtacos_periodic_checks(Uint32 interval, void *param __attribute__((unused))) {
  static int i = 0;

  SDL_mutexP(mutex);
  if (SDL_QuitRequested()) {
    exit(0);
  }
  SDL_mutexV(mutex);

  if (i % 5 == 0) {
    SDL_Event events[MAX_SDL_EVENTS];
    int nb_pending_events = SDL_PeepEvents(events, MAX_SDL_EVENTS, SDL_PEEKEVENT, SDL_ALLEVENTS);
    //printf("[Leek monitor] %d pending event(s).\n", nb_pending_events);
    if (nb_pending_events >= MAX_SDL_EVENTS - 1) {
      printf("[Leek monitor] Event queue full - exiting.\n");
      exit(1);
    }
  }

  i = (i + 1) % 5;

  return interval;
}

void init_hooks() {
  libc_open = dlsym(RTLD_NEXT, "open");
  libc_close = dlsym(RTLD_NEXT, "close");
  libc_read = dlsym(RTLD_NEXT, "read");
  libc_ioctl = dlsym(RTLD_NEXT, "ioctl");
}

void init_sdl() {
  mutex = SDL_CreateMutex();

  SDL_mutexP(mutex);

  if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER) < 0) {
    fprintf(stderr, "Vous n'avez pas compris les concepts : %s\n", SDL_GetError());
    exit(1);
  }

  SDL_EventState(0xFF, SDL_IGNORE); // Ignore all events
  SDL_EventState(SDL_QUIT, SDL_ENABLE);
  SDL_EventState(SDL_KEYDOWN, SDL_ENABLE);
  SDL_EventState(SDL_KEYUP, SDL_ENABLE);
  SDL_EnableUNICODE(SDL_ENABLE);
  SDL_ShowCursor(SDL_DISABLE);
  SDL_WM_SetCaption("TacOS Emulator", "TacOS Emulator");

  SDL_mutexV(mutex);

  SDL_AddTimer(1000, libtacos_periodic_checks, NULL);

  SDL_initialized = 1;
}

void destroy_sdl() {
  SDL_Quit();
}

void __attribute__((constructor)) libtacos_init() {
  printf("Initializing libtacos.\n");
  init_hooks();
}

void __attribute__((destructor)) libtacos_destroy() {
  printf("Destroying libtacos.\n");
  if (SDL_initialized) {
    SDL_Quit();
  }
}

int open(const char *pathname, int flags) {
  int fd;

  //printf("open %s %d!\n", pathname, flags);

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

int close(int fd) {
  //printf("close!\n");

  return libc_close(fd);
}

ssize_t read(int fd, void *buf, size_t count) {
  int ret = -1;

  //printf("read!\n");

  switch (tacos_descriptors[fd]) {
    case NONE: {
      ret = libc_read(fd, buf, count);
      break;
    }
    case VGA: {
      ret = -1;
      break;
    }
    case VESA: {
      ret = -1;
      break;
    }
    case MOUSE: {
      ret = mouse_read(buf);
      break;
    }
    case KEYBOARD: {
      ret = kbd_read(buf);
    }
  }

  return ret;
}

int ioctl(int fd, unsigned long request, void *data) {
  int ret = -1;

  //printf("ioctl!\n");
  
  switch (tacos_descriptors[fd]) {
    case NONE: {
      ret = libc_ioctl(fd, request, data);
      break;
    }
    case VGA: {
      ret = vga_ioctl(request, data);
      break;
    }
    case VESA: {
      ret = vesa_ioctl(request, data);
      break;
    }
    case MOUSE: {
      ret = mouse_ioctl(request, data);
      break;
    }
    case KEYBOARD: {
      ret = kbd_ioctl(request, data);
      break;
    }
  }

  return ret;
}

int getchar() {
  int ret;
  SDL_mutexP(mutex);
  SDL_PumpEvents();
  SDL_mutexV(mutex);
  SDL_Event event;
  if (SDL_PeepEvents(&event, 1, SDL_GETEVENT, SDL_KEYDOWNMASK)) {
    //printf("%d %d\n", event.key.keysym.scancode, event.key.keysym.unicode);
    ret = event.key.keysym.unicode;
  } else {
    ret = EOF;
  }
  //printf("getchar %d\n", ret);
  return ret;
}

void debug(const char * format, ...) {
#ifdef DEBUG
  va_list args;
  va_start(args, format);
  vprintf(format, args);
  va_end(args);
#endif // DEBUG
}

int exec_elf(char* cmdline, int orphan __attribute__((unused))) {
  char cmdline2[strlen(cmdline) + 3];
  if (strcpy(cmdline2, cmdline) == NULL) {
    return -1;
  }
  if (strcat(cmdline2, " &") == NULL) {
    return -1;
  }
  return system(cmdline2);
}
