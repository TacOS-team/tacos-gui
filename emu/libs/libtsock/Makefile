SRCS = *.c

all: $(LIBTSOCK)

$(LIBTSOCK): $(BUILDDIROBJS)/tsock.o
	$(LD) -o $@ -shared $< -lc

$(BUILDDIROBJS)/%.o: %.c
	$(CC) -o $@ -c $< $(CFLAGS) $(INCLUDES) -fPIC

depend: $(SRCS)
	$(CC) -MM $(CFLAGS) $(INCLUDES) $^ > .depend.tmp
	sed -E 's#^([^:]+):#$(BUILDDIROBJS)/\1:#' .depend.tmp > .depend
	rm .depend.tmp

-include .depend
