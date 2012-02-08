INCLUDES=-I. -I./include/ -I../include/
CFLAGS=-Wall -g
BUILDDIR=../build
BUILDDIROBJECT=$(BUILDDIR)/objects

SRCS=*.c

all:  $(BUILDDIROBJECT)/vga.o \
			$(BUILDDIROBJECT)/vesa.o \
			$(BUILDDIROBJECT)/mouse.o

$(BUILDDIROBJECT)/%.o: %.c
	$(CC) -o $@ -c $< $(CFLAGS) $(INCLUDES) -fPIC

clean:
	rm -f $(BUILDDIROBJECT)/*

.PHONY: clean
