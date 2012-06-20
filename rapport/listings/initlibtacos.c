int (*libc_open)(const char *pathname, int flags);
int (*libc_close)(int fd);
ssize_t (*libc_read)(int fd, void *buf, size_t count);
int (*libc_ioctl)(int fd, unsigned long request, void *data);

void __attribute__((constructor)) libtacos_init() {
  printf("Initializing libtacos.\n");
  init_hooks();
}

void init_hooks() {
  libc_open = dlsym(RTLD_NEXT, "open");
  libc_close = dlsym(RTLD_NEXT, "close");
  libc_read = dlsym(RTLD_NEXT, "read");
  libc_ioctl = dlsym(RTLD_NEXT, "ioctl");
}
