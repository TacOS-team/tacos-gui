SRCS = *.c
OBJS = vgadrv.o mousedrv.o vesadrv.o kbddrv.o

all: $(LIBTACOS)

$(LIBTACOS): $(BUILDDIROBJS)/libtacos.o
	$(CC) -o $@ -shared $< $(addprefix $(BUILDDIROBJS)/, $(OBJS)) -lSDL -ldl -lc

$(BUILDDIROBJS)/%.o: %.c
	$(CC) -o $@ -c $< $(CFLAGS) $(INCLUDES) -fPIC

depend: $(SRCS)
	$(CC) -MM $(CFLAGS) $(INCLUDES) $^ > .depend.tmp
	sed -E 's#^([^:]+):#$(BUILDDIROBJS)/\1:#' .depend.tmp > .depend
	rm .depend.tmp

-include .depend
