SRCS = *.c
OBJS = libtacos.o vgadrv.o mousedrv.o vesadrv.o kbddrv.o

all: $(LIBTACOS)

$(LIBTACOS): $(addprefix $(BUILDDIROBJS)/, $(OBJS))
	$(CC) -o $@ -shared $^ -lSDL -ldl -lc

$(BUILDDIROBJS)/%.o: %.c
	$(CC) -o $@ -c $< $(CFLAGS) $(INCLUDES) -fPIC

depend: $(SRCS)
	$(CC) -MM $(CFLAGS) $(INCLUDES) $^ > .depend.tmp
	sed -E 's#^([^:]+):#$(BUILDDIROBJS)/\1:#' .depend.tmp > .depend
	rm .depend.tmp

-include .depend
