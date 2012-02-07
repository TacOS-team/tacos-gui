INCLUDES= #-I./include/
CFLAGS=-Wall -g
#LIBTACOS=$(realpath .)/libtacos.so
#export LIBTACOS

all: x_danny

x_danny: x_danny.o 
	$(CC)  $< -o $@ -lX11

%.o: %.c
	$(CC) -o $@ -c $< $(CFLAGS) $(INCLUDES) -lX11

clean:
	rm -f *.o

.PHONY: x_danny clean
