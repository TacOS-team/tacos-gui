all: applications libtacos

applications: force_look
	$(MAKE) $(MFLAGS) -C applications

drivers: force_look
	$(MAKE) $(MFLAGS) -C drivers

libtacos: libtacos.o drivers
	$(LD) -o libtacos.so -shared drivers/vga.o drivers/mouse.o libtacos.o -lSDL -ldl

force_look:
	@true

runcube:
	LD_PRELOAD=./libtacos.so applications/cube_graph

runcubev2:
	LD_PRELOAD=./libtacos.so applications/cube_graph_mouse

runzen:
	LD_PRELOAD=./libtacos.so applications/zen_graph
