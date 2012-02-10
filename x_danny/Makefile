INCLUDES= #-I./include/
CFLAGS=-Wall -g
#LIBTACOS=$(realpath .)/libtacos.so
#export LIBTACOS

all: x_danny xeyes

x_danny: x_danny.o images.o
	$(CC) $^ -o $@ -lX11 -lm

xeyes: xeyes.o
	$(CC) $^ -o $@ -lSDL

%.o: %.c
	$(CC) -o $@ -c $< $(CFLAGS) $(INCLUDES)

clean:
	rm -f *.o

.PHONY: x_danny clean
