int ioctl(int fd, unsigned long request, void *data) {
  int ret = -1;

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
