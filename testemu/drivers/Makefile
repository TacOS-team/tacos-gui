INCLUDES=-I. -I./include/ -I../include/
CFLAGS=-Wall -g

SRCS=*.c

all: vga.o mouse.o

%.o: %.c
	$(CC) -o $@ -c $< $(CFLAGS) $(INCLUDES) 

clean:
	rm -f *.o

.PHONY: clean
