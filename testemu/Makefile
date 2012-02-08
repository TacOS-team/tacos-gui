INCLUDES= -I./include/
CFLAGS=-Wall -g
LIBTACOS=$(realpath .)/libtacos.so
export LIBTACOS

all: applications

applications: libtacos
	$(MAKE) $(MFLAGS) -C applications

drivers:
	$(MAKE) $(MFLAGS) -C drivers

libtacos: libtacos.o drivers
	$(LD) -o $(LIBTACOS) -shared drivers/*.o libtacos.o -lSDL -ldl

%.o: %.c
	$(CC) -o $@ -c $< $(CFLAGS) $(INCLUDES) -fPIC

clean:
	$(MAKE) $@ $(MFLAGS) -C applications
	$(MAKE) $@ $(MFLAGS) -C drivers
	rm -f *.o libtacos.so

.PHONY: applications drivers clean
