SRCS = *.c
OBJS = vgadrv.o vesadrv.o mousedrv.o kbddrv.o

all: $(addprefix $(BUILDDIROBJS)/, $(OBJS))

$(BUILDDIROBJS)/%.o: %.c
	$(CC) -o $@ -c $< $(CFLAGS) $(INCLUDES) -fPIC

depend: $(SRCS)
	$(CC) -MM $(CFLAGS) $(INCLUDES) $^ > .depend.tmp
	sed -E 's#^([^:]+):#$(BUILDDIROBJS)/\1:#' .depend.tmp > .depend
	rm .depend.tmp

.PHONY: clean

-include .depend
