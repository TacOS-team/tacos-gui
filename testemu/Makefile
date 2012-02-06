INCLUDES= -I./include/
CFLAGS=-Wall -g

%.o: %.c
	$(CC) -o $@ -c $< $(CFLAGS) $(INCLUDES) -fPIC

all: applications libtacos

applications:
	$(MAKE) $(MFLAGS) -C applications

drivers:
	$(MAKE) $(MFLAGS) -C drivers

libtacos: libtacos.o drivers
	$(LD) -o libtacos.so -shared drivers/vga.o drivers/mouse.o libtacos.o -lSDL -ldl

clean:
	$(MAKE) $@ $(MFLAGS) -C applications
	$(MAKE) $@ $(MFLAGS) -C drivers
	rm -f *.o libtacos.so

runcube:
	LD_PRELOAD=./libtacos.so applications/cube_graph

runcubev2:
	LD_PRELOAD=./libtacos.so applications/cube_graph_mouse

runzen:
	LD_PRELOAD=./libtacos.so applications/zen_graph

runpong:
	LD_PRELOAD=./libtacos.so applications/pong

runsnake:
	LD_PRELOAD=./libtacos.so applications/snake_graph

.PHONY: applications drivers clean runcube runcubev2 runzen
