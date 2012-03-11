#ifndef _KBD_TYPES_H_
#define _KBD_TYPES_H_

typedef struct kbdstate {
  unsigned char state;
  unsigned int keysym;
  unsigned int modifiers;
} kbdstate_t;

#endif