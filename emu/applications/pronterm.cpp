#include <fcntl.h>
#include <unistd.h>
#include <pronlib.h>
#include <errno.h>

#define NB_FONTS 5

using namespace pron;

Display *d = pronConnect();
Window w;
Pixmap p;
int fontNum = 0;
PronGCValues backgroundValues = {Color(200, 200, 200), Color(200, 200, 200), 3};
PronGCValues foregroundValues = {Color(0, 0, 0), Color(200, 200, 200), 3};
GC backgroundGC = pronCreateGC(d, backgroundValues, GC_VAL_FG | GC_VAL_FONTNUM);
GC foregroundGC = pronCreateGC(d, foregroundValues, GC_VAL_FG | GC_VAL_FONTNUM);

int width = 600, height = 400;
int posX, posY;
unsigned char attr;
int textWidth, textHeight;
int master, slave;

enum ansi_color {
  BLACK,
  RED,
  GREEN,
  BLUE,
  CYAN,
  MAGENTA,
  BROWN,
  LIGHT_GRAY,
  DARK_GRAY,
  LIGHT_RED,
  LIGHT_GREEN,
  LIGHT_BLUE,
  LIGHT_CYAN,
  LIGHT_MAGENTA,
  YELLOW,
  WHITE,
};

Color colors[] = {
  Color(0x00, 0x00, 0x00),
  Color(0xcc, 0x00, 0x00),
  Color(0x00, 0xcc, 0x00),
  Color(0x00, 0x00, 0xcc),
  Color(0x00, 0xcc, 0xcc),
  Color(0xcc, 0x00, 0xcc),
  Color(0xcc, 0xcc, 0x00),
  Color(0xfa, 0xeb, 0xd7),
  Color(0x33, 0x33, 0x33),
  Color(0xff, 0x00, 0x00),
  Color(0x00, 0xff, 0x00),
  Color(0x00, 0x00, 0xff),
  Color(0x00, 0xff, 0xff),
  Color(0xff, 0x00, 0xff),
  Color(0xff, 0xff, 0x00),
  Color(0xff, 0xff, 0xff),
};

void drawChar(unsigned char c);

void cursorUp() {
  if (posY > 0) {
    posY -= textHeight;
  }
}

void cursorDown() {
  if (posY < height - textHeight) {
    posY += textHeight;
  }
}

void cursorBack() {
  if (posX > 0) {
    posX -= textWidth;
  }
}

void cursorForward() {
  if (posY < width - textWidth) {
    posX += textWidth;
  }
}

void newline() {
  printf("newline\n");
  posX = 0;
  posY += textHeight;
  if (posY + textHeight > height) {
    posY -= textHeight;
    pronCopyArea(d, p, p, foregroundGC, 0, 0, width, height, 0, -textHeight);
    pronFillRectangle(d, p, backgroundGC, 0, posY, width, textHeight);
    pronCopyArea(d, p, w, d->defaultGC, 0, 0, width, height, 0, 0);
  }
  //foregroundValues.font_num = (foregroundValues.font_num + 1) % 5;
  //pronChangeGC(d, foregroundGC, foregroundValues, GC_VAL_FONTNUM);
}

void lineup() {
  printf("lineup\n");
  posX = 0;
  cursorUp();
}

void cursorMoveCol(unsigned int x) {
  if ((x - 1) * textWidth < width - textWidth) {
    posX = (x - 1) * textWidth;
  }
}

void cursorMove(unsigned int y, unsigned int x) {
  if ((y - 1) * textHeight < height - textHeight) {
    posY = (y - 1) * textHeight;
  }
  if ((x - 1) * textWidth < width - textWidth) {
    posX = (x - 1) * textWidth;
  }
}

void clearConsole() {
  printf("clear\n");
  posX = 0;
  posY = 0;
  pronFillRectangle(d, p, backgroundGC, 0, 0, width, height);
  pronCopyArea(d, p, w, d->defaultGC, 0, 0, width, height, 0, 0);
}

void setForeground(uint8_t foreground) {
  printf("setforeground %d\n", foreground);
  foregroundValues.fg = colors[foreground];
  pronChangeGC(d, foregroundGC, foregroundValues, GC_VAL_FG);
}

void setBackground(uint8_t background) {
  printf("setbackground %d\n", background);
  foregroundValues.bg = colors[background];
  pronChangeGC(d, foregroundGC, foregroundValues, GC_VAL_BG);
}

void resetAttribute() {
  printf("reset\n");
  foregroundValues.fg = Color(0, 0, 0);
  foregroundValues.bg = Color(200, 200, 200);
  pronChangeGC(d, foregroundGC, foregroundValues, GC_VAL_FG | GC_VAL_BG);
}

void backspace() {
  printf("backspace\n");
  if (posX > 0) {
    posX -= textWidth;
    pronFillRectangle(d, p, backgroundGC, posX, posY, textWidth, textHeight);
    pronCopyArea(d, p, w, d->defaultGC, posX, posY, textWidth, textHeight, posX, posY);
  }
}

void tab() {
  posX += textWidth;
  while ((posX / textWidth) & 7) {
    drawChar(' ');
  }
}

void drawChar(unsigned char c) {
  //printf("drawchar %d: '%c'\n", c, c);
  static bool escapeChar = false;
  static bool ansiEscapeCode = false;
  static bool ansiSecondVal = false;

  static int val, val2 = 0;

  static bool bright = false;

  if (escapeChar) {
    if (ansiEscapeCode) {
      if (c >= '0' && c <= '9') {
        if (ansiSecondVal) {
          val2 = val2 * 10 + c - '0';
        } else {
          val = val * 10 + c - '0';
        }
      } else { 
        escapeChar = false;
        ansiSecondVal = false;
        ansiEscapeCode = false;
        if (val == 0 && c != 'J' && c != 'm')
          val = 1;
        if (val2 == 0)
          val2 = 1;
        switch (c) {
          case 'A': // CUU cursor up
            while (val--)
              cursorUp();
            break;
          case 'B': // CUD cursor down
            while (val--)
              cursorDown();
            break;
          case 'C': // CUF cursor forward
            while (val--)
              cursorForward();
            break;
          case 'D': // CUB cursor back
            while (val--)
              cursorBack();
            break;
          case 'E': // CNL cursor next line
            while (val--)
              newline();
            break;
          case 'F': // CPL cursor previous line
            while (val--)
              lineup();
            break;
          case 'G': // CHA cursor horizontal absolute
            cursorMoveCol(val);
            break;
          case 'f': 
          case 'H': // CUP cursor position
            cursorMove(val, val2);
            break;
          case 'J': // ED erase data
            if (val == 0) {
              // from cursor to end of screen
            } else if (val == 1) {
              // from cursor to beginning of screen
            } else if (val == 2) {
              // entire screen
              clearConsole();
            }
            break;
          case ';':
            escapeChar = true;
            ansiSecondVal = true;
            ansiEscapeCode = true;
            val2 = 0;
            break;
          case 'm': // SGR select graphic rendition
          lol:
            if (val == 0) {
              resetAttribute();
            } else if (val == 1) {
              bright = 1;
            } else if (val == 2) {
              bright = 0;
            } else if (val >= 30 && val <= 37) {
              // si low intensity (normal) :
              if (bright == 0) {
                switch (val - 30) {
                  case 0:
                    setForeground(BLACK);
                    break;
                  case 1:
                    setForeground(RED);
                    break;
                  case 2:
                    setForeground(GREEN);
                    break;
                  case 3:
                    setForeground(BROWN);
                    break;
                  case 4:
                    setForeground(BLUE);
                    break;
                  case 5:
                    setForeground(MAGENTA);
                    break;
                  case 6:
                    setForeground(CYAN);
                    break;
                  case 7:
                    setForeground(LIGHT_GRAY);
                    break;
                }
              } else {
                switch (val - 30) {
                  case 0:
                    setForeground(DARK_GRAY);
                    break;
                  case 1:
                    setForeground(LIGHT_RED);
                    break;
                  case 2:
                    setForeground(LIGHT_GREEN);
                    break;
                  case 3:
                    setForeground(YELLOW);
                    break;
                  case 4:
                    setForeground(LIGHT_BLUE);
                    break;
                  case 5:
                    setForeground(LIGHT_MAGENTA);
                    break;
                  case 6:
                    setForeground(LIGHT_CYAN);
                    break;
                  case 7:
                    setForeground(WHITE);
                    break;
                }
              }
            } else if (val >= 40 && val <= 47) {
              // low intensity (normal) :
              switch (val - 40) {
                case 0:
                  setBackground(BLACK);
                  break;
                case 1:
                  setBackground(RED);
                  break;
                case 2:
                  setBackground(GREEN);
                  break;
                case 3:
                  setBackground(BROWN);
                  break;
                case 4:
                  setBackground(BLUE);
                  break;
                case 5:
                  setBackground(MAGENTA);
                  break;
                case 6:
                  setBackground(CYAN);
                  break;
                case 7:
                  setBackground(LIGHT_GRAY);
                  break;
              }
            } else if (val >= 90 && val <= 99) {
              switch (val - 90) {
                case 0:
                  setForeground(DARK_GRAY);
                  break;
                case 1:
                  setForeground(LIGHT_RED);
                  break;
                case 2:
                  setForeground(LIGHT_GREEN);
                  break;
                case 3:
                  setForeground(YELLOW);
                  break;
                case 4:
                  setForeground(LIGHT_BLUE);
                  break;
                case 5:
                  setForeground(LIGHT_MAGENTA);
                  break;
                case 6:
                  setForeground(LIGHT_CYAN);
                  break;
                case 7:
                  setForeground(WHITE);
                  break;
              }
            } else if (val >= 100 && val <= 109) {
              switch (val - 100) {
                case 0:
                  setBackground(DARK_GRAY);
                  break;
                case 1:
                  setBackground(LIGHT_RED);
                  break;
                case 2:
                  setBackground(LIGHT_GREEN);
                  break;
                case 3:
                  setBackground(YELLOW);
                  break;
                case 4:
                  setBackground(LIGHT_BLUE);
                  break;
                case 5:
                  setBackground(LIGHT_MAGENTA);
                  break;
                case 6:
                  setBackground(LIGHT_CYAN);
                  break;
                case 7:
                  setBackground(WHITE);
                  break;
              }
            }
            if (val2 != 0) {
              val = val2;
              val2 = 0;
              goto lol;
            }
            break;
        }
      }
    } else {
      if (c == '[') {
        ansiEscapeCode = true;
        val = 0;
      } else {
        escapeChar = false;
      }
    }
  } else if (c == '\033') {
    escapeChar = true;
  } else if (c == '\n') {
    newline();
  } else if (c == '\r') {
    printf("got \\r\n");
    //posX = 0;
  } else if (c == '\t') {
    tab();
  } else if (c == '\b') {
    backspace();
  } else {
    printf("posX = %d\n", posX);
    if (posX >= width) {
      printf("%d >= %d, newline\n", posX, width);
      newline();
    }
    pronDrawText(d, p, foregroundGC, posX, posY, (char*) &c, 1);
    pronCopyArea(d, p, w, d->defaultGC, posX, posY, textWidth, textHeight, posX, posY);
    posX += textWidth;
  }
}

int setupTerm() {
  char *name;

  if ((master = getpt()) < 0) {
    perror("getpt");
    return -1;
  }
  
  if (grantpt(master) || unlockpt(master) || !(name = ptsname(master))) {
    perror("grantpt/unlockpt/ptsname");
    return -1;
  }

  int flags = fcntl(master, F_GETFL, 0);
  if (fcntl(master, F_SETFL, flags | O_NONBLOCK) < 0) {
    perror("fcntl");
    return -1;
  }

  switch (fork()) {
    case -1: {
      perror("fork");
      return -1;
    }
    case 0: {
      if ((slave = open(name, O_RDWR)) < 0) {
        perror("open slave");
        exit(1);
      }

      close(STDIN_FILENO);
      close(STDOUT_FILENO);
      close(STDERR_FILENO);
      close(master);
      
      dup2(slave, STDIN_FILENO);
      dup2(slave, STDOUT_FILENO);
      dup2(slave, STDERR_FILENO);

      char *argv[2] = {(char*) "/bin/bash", NULL};
      execv(argv[0], argv);

      exit(1);
      break;
    }
    default: {
      break;
    }
  }

  return 0;
}

int main() {
  if (d == NULL) {
    fprintf(stderr, "Unable to connect to pron.\n");
    exit(1);
  }

  w = pronCreateWindow(d, d->rootWindow, 50, 50, width, height);
  pronSelectInput(d, w, PRON_EVENTMASK(EV_EXPOSE) |
      PRON_EVENTMASK(EV_DESTROY_WINDOW) | PRON_EVENTMASK(EV_KEY_PRESSED));

  pronMapWindow(d, w);
  
  p = pronCreatePixmap(d, width, height, 24);

  pronTextSize(d, d->defaultGC, "A", 1, &textWidth, &textHeight);

  clearConsole();

  if (setupTerm() < 0) {
    fprintf(stderr, "fuck!\n");
    exit(1);
  }

  PronEvent *event = getPronEvent();

  char ch;
  while (1) {
    if (pronNextEvent(d, event, true)) {
      switch (event->type) {
        case EV_KEY_PRESSED: {
          EventKeyPressed *keyPressedEvent = (EventKeyPressed*) event;
          ch = keyPressedEvent->ch;
          if (ch != 0) {
            write(master, &ch, 1);
          }
          break;
        }
        case EV_DESTROY_WINDOW: {
          EventDestroyWindow *destroyWindowEvent = (EventDestroyWindow*) event;
          printf("Bye!\n");
          if (destroyWindowEvent->window == w) {
            pronDisconnect(d);
          }
          return 0;
          break;
        }
        case EV_EXPOSE: {
          pronCopyArea(d, p, w, d->defaultGC, 0, 0, width, height, 0, 0);
          break;
        }
        default:
          break;
      }
    } else {
      if (errno != EAGAIN/* && errno != EWOULDBLOCK*/) {
        fprintf(stderr, "pron has closed the connection.\n");
        exit(1);
      }
    }

    if (read(master, &ch, 1) > 0) {
      drawChar(ch);
    }

    usleep(1000);
  }

  pronDestroyWindow(d, w);

  pronDisconnect(d);

  return 0;
}
