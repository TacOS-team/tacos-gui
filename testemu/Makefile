INCLUDES= -I./include/
CFLAGS=-Wall -g

%.o: %.c
	$(CC) -o $@ -c $< $(CFLAGS) $(INCLUDES) -fPIC

all: libtacos applications

applications:
	$(MAKE) $(MFLAGS) -C applications

drivers:
	$(MAKE) $(MFLAGS) -C drivers

libtacos: libtacos.o drivers
	$(LD) -o libtacos.so -shared drivers/*.o libtacos.o -lSDL -ldl

clean:
	$(MAKE) $@ $(MFLAGS) -C applications
	$(MAKE) $@ $(MFLAGS) -C drivers
	rm -f *.o libtacos.so

runcube: applications libtacos
	LD_PRELOAD=./libtacos.so applications/cube_graph

runcubev2: applications libtacos
	LD_PRELOAD=./libtacos.so applications/cube_graph_mouse

runzen: applications libtacos
	LD_PRELOAD=./libtacos.so applications/zen_graph

runpong: applications libtacos
	LD_PRELOAD=./libtacos.so applications/pong

runsnake: applications libtacos
	LD_PRELOAD=./libtacos.so applications/snake_graph

runclock: applications libtacos
	LD_PRELOAD=./libtacos.so applications/clock_graph

.PHONY: applications drivers clean
