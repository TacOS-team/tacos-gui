#define _GNU_SOURCE
#include <dlfcn.h>
#include <signal.h>
#include <stdio.h>
#include <unistd.h>
#include <SDL/SDL.h>

#define MAX_FD 128

SDL_Surface *screen;

int (*libc_open)(const char *pathname, int flags);
int (*libc_close)(int fd);
ssize_t (*libc_read)(int fd, void *buf, size_t count);
int (*libc_ioctl)(int fd, unsigned long request, void *data);

enum tacos_descriptor_type {
	NONE,
	VGA,
	MOUSE,
};

enum tacos_descriptor_type tacos_descriptors[MAX_FD];

void alarm_handler(int signum) {
	if (SDL_QuitRequested()) {
		exit(0);
	}

	alarm(1);
}

void __attribute__((constructor)) init() {
	printf("Initializing hooklib.\n");
	libc_open = dlsym(RTLD_NEXT, "open");
	libc_close = dlsym(RTLD_NEXT, "close");
	libc_read = dlsym(RTLD_NEXT, "read");
	libc_ioctl = dlsym(RTLD_NEXT, "ioctl");

	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		fprintf(stderr, "Vous n'avez pas compris les concepts : %s\n", SDL_GetError());
		exit(1);
	}

	SDL_EventState(0xFF, SDL_IGNORE); // Ignore all events
	SDL_EventState(SDL_QUIT, SDL_ENABLE);
	SDL_EventState(SDL_KEYDOWN, SDL_ENABLE);
	SDL_EnableUNICODE(SDL_ENABLE);
	SDL_ShowCursor(SDL_DISABLE);
	SDL_WM_SetCaption("TacOS Emulator", "TacOS Emulator");

	signal(SIGALRM, alarm_handler);
	alarm(1);
}

void __attribute__((destructor)) destroy() {
	printf("Destroying hooklib.\n");
	SDL_Quit();
}

int open(const char *pathname, int flags) {
	int fd;

	printf("open %s %d!\n", pathname, flags);

	if (strcmp(pathname, "/dev/vga") == 0) {
		// VGA driver
		fd = libc_open("/tmp", 0); // dummy fd
		tacos_descriptors[fd] = VGA;
	} else if (strcmp(pathname, "/dev/mouse") == 0) {
		// Mouse driver
		fd = libc_open("/tmp", 0); // dummy fd
		tacos_descriptors[fd] = MOUSE;
	} else {
		// Native open
		fd = libc_open(pathname, flags);
	}
	
	return fd;
}

int close(int fd) {
	printf("close!\n");

	return libc_close(fd);
}

ssize_t read(int fd, void *buf, size_t count) {
	int ret;

	printf("read!\n");

	switch (tacos_descriptors[fd]) {
		case NONE: {
			ret = libc_read(fd, buf, count);
			break;
		}
		case VGA: {
			ret = -1;
			break;
		}
		case MOUSE: {
			ret = mouse_read(buf);
			break;
		}
	}

	return ret;
}

int ioctl(int fd, unsigned long request, void *data) {
	int ret;

	printf("ioctl!\n");
	
	switch (tacos_descriptors[fd]) {
		case NONE: {
			ret = libc_ioctl(fd, request, data);
			break;
		}
		case VGA: {
			ret = vga_ioctl(request, data);
			break;
		}
		case MOUSE: {
			ret = mouse_ioctl(request, data);
			break;
		}
	}

	return ret;
}

int getchar() {
	int ret;
	SDL_PumpEvents();
	SDL_Event event;
	if (SDL_PeepEvents(&event, 1, SDL_GETEVENT, SDL_KEYDOWNMASK)) {
		printf("%d %d\n", event.key.keysym.scancode, event.key.keysym.unicode);
		ret = event.key.keysym.unicode;
	} else {
		ret = EOF;
	}
	printf("getchar %d\n", ret);
	return ret;
}
