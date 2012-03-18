/**
 * @file snake.c
 *
 * @author TacOS developers 
 *
 * @section LICENSE
 *
 * Copyright (C) 2010 - TacOS developers.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 3 of
 * the License, or (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details at
 * http://www.gnu.org/copyleft/gpl.html
 *
 * You should have received a copy of the GNU General Public License 
 * along with this program; if not, see <http://www.gnu.org/licenses>.
 *
 * @section DESCRIPTION
 *
 * Description de ce que fait le fichier
 */

#include <cstdio>
#include <signal.h>
#include <unistd.h>
#include <pronlib.h>
#include <time.h>
#include <errno.h>

#define LARGEUR 320
#define HAUTEUR 200

#define FACTEUR 4

#define LIGNES 50
#define COLONNES 80

Display *d;
Window w;
GC backgroundGC, bleuGC, roseGC;

PronEvent *e;

static int dir = 2, dir_ = 2;
static int score = 0;

void drawPoint(int x, int y, GC gc) {
  pronFillRectangle(d, w, gc, x * FACTEUR, y * FACTEUR, FACTEUR, FACTEUR);
}

struct coord_t {
  int l, c;
};

struct snake_t {
  int longueur;
  int indice_deb;
  struct coord_t coords[LIGNES * COLONNES];
};

static struct snake_t snake;
static struct coord_t directions[4] = {{-1, 0},{1, 0},{0, 1}, {0, -1}};

static struct coord_t bonus;

static void haut() {
  if (dir_ != 1) {
    dir = 0;
  }
}

static void bas() {
  if (dir_ != 0) {
    dir = 1;
  }
}

static void droite() {
  if (dir_ != 3) {
    dir = 2;
  }
}

static void gauche() {
  if (dir_ != 2) {
    dir = 3;
  }
}

static int collision(int l, int c, int n) {
  for (int i = 0; i < snake.longueur - n; i++) {
    if (l == snake.coords[(snake.indice_deb + i) % (LIGNES * COLONNES)].l &&
      c == snake.coords[(snake.indice_deb + i) % (LIGNES * COLONNES)].c) {
      return 1;
    }
  }
  return 0;
}

static void genere_bonus() {
  do {
    bonus.l = (rand() % (LIGNES - 2 + 1)) + 1;
    bonus.c = (rand() % (COLONNES - 2 + 1)) + 1;
  } while (collision(bonus.l, bonus.c, 0));

  drawPoint(bonus.c, bonus.l, roseGC);
}

void init_snake() {
  pronFillRectangle(d, w, bleuGC, 0, (LIGNES - 1) * FACTEUR, COLONNES * FACTEUR, FACTEUR);
  pronFillRectangle(d, w, bleuGC, 0, 0, FACTEUR, LIGNES * FACTEUR);
  pronFillRectangle(d, w, bleuGC, (COLONNES - 1) * FACTEUR, 0, FACTEUR, LIGNES * FACTEUR);
  pronFillRectangle(d, w, bleuGC, 0, 0, COLONNES * FACTEUR, FACTEUR);

  dir = 2;
  dir_ = 2;
  snake.longueur = 1;
  snake.indice_deb = 0;
  snake.coords[snake.indice_deb].c = COLONNES/3;
  snake.coords[snake.indice_deb].l = LIGNES/2;

  drawPoint(snake.coords[snake.indice_deb].c, snake.coords[snake.indice_deb].l, bleuGC);

  genere_bonus();
}

static inline int dans_aire_jeu(int l, int c) {
  int ok = (l > 0 && l < LIGNES -1 && c > 0 && c < COLONNES - 1);
  return ok;
}

int avance_snake() {
  static int allonge = 0;
  if (snake.longueur < 4 || allonge) {
    snake.longueur++;
    allonge = 0;
  } else {
    drawPoint(snake.coords[snake.indice_deb].c, snake.coords[snake.indice_deb].l, backgroundGC);
    snake.indice_deb = (snake.indice_deb + 1) % (LIGNES * COLONNES);
  }

  int i = (snake.indice_deb + snake.longueur - 1) % (LIGNES * COLONNES);
  int i_ = (snake.indice_deb + snake.longueur - 2) % (LIGNES * COLONNES);
  snake.coords[i].l = snake.coords[i_].l + directions[dir].l;
  snake.coords[i].c = snake.coords[i_].c + directions[dir].c;

  dir_ = dir;

  if (snake.coords[i].l == bonus.l && snake.coords[i].c == bonus.c) {
    score += 1;
    genere_bonus();
    allonge = 1;
  }

  if (!dans_aire_jeu(snake.coords[i].l, snake.coords[i].c) || collision(snake.coords[i].l, snake.coords[i].c, 1)) {
    return -1;
  }

  drawPoint(snake.coords[i].c, snake.coords[i].l, bleuGC);

  return 0;
}

void thread_input() {
  if (!pronNextEvent(d, e, true)) {
    if (errno != EAGAIN && errno != EWOULDBLOCK) {
      perror("pron has closed the connection");
      exit(1);
    } else {
      return;
    }
  }

  switch (e->type) {
    case EV_KEY_PRESSED : {
      EventKeyPressed *keyPressed = (EventKeyPressed*) e;
      switch (keyPressed->keysym) {
        case PRONK_z:
          haut();
          break;
        case PRONK_s:
          bas();
          break;
        case PRONK_d:
          droite();
          break;
        case PRONK_q:
          gauche();
          break;
      }
      break;
    }
    default:
      break;
  }
}

void game(int x, int y) {
  d = pronConnect();
  if (d == NULL) {
    fprintf(stderr, "Unable to connect to pron.\n");
    exit(1);
  }

  w = pronCreateWindow(d, d->rootWindow, x, y, 320, 200);

  color_t bgColor, bleu, rose;
  COLOR(bgColor, 24).r = (w >> 16) << 3;
  COLOR(bgColor, 24).g = (w >> 16) << 3;
  COLOR(bgColor, 24).b = (w >> 16) << 3;

  COLOR(rose, 24).r = 255;
  COLOR(rose, 24).g = 77;
  COLOR(rose, 24).b = 182;

  COLOR(bleu, 24).r = 77;
  COLOR(bleu, 24).g = 182;
  COLOR(bleu, 24).b = 255;

  PronGCValues values;
  values.fg = bgColor;
  backgroundGC = pronCreateGC(d, values, GC_VAL_FG);
  values.fg = bleu;
  bleuGC = pronCreateGC(d, values, GC_VAL_FG);
  values.fg = rose;
  roseGC = pronCreateGC(d, values, GC_VAL_FG);

  PronWindowAttributes newAttr;
  newAttr.bgColor = bgColor;
  pronSetWindowAttributes(d, w, newAttr, WIN_ATTR_BG_COLOR);

  pronMapWindow(d, w);

  init_snake();

  // Subscribe to window creation events
  pronSelectInput(d, d->rootWindow, PRON_EVENTMASK(EV_KEY_PRESSED));

  e = getPronEvent();

  while (avance_snake() != -1) {
    usleep(100000);
    thread_input();
  }

  pronDisconnect(d);
}

int main(int argc, char *argv[]) {
  srand(time(NULL));

  int x, y;

  if (argc > 2) {
    x = atoi(argv[1]);
    y = atoi(argv[2]);
    printf("Position: %d %d\n", x, y);
  } else {
    x = 20;
    y = 10;
  }

  game(x, y);

  return 0;
}
