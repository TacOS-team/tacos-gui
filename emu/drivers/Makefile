INCLUDES=-I. -I./include/ -I../include/
CFLAGS=-Wall -g

SRCS=*.c

all: vga.o vesa.o mouse.o

%.o: %.c
	$(CC) -o $@ -c $< $(CFLAGS) $(INCLUDES) -fPIC

clean:
	rm -f *.o

.PHONY: clean
