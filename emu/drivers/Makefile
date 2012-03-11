OBJS = vgadrv.o vesadrv.o mousedrv.o kbddrv.o

all: $(addprefix $(BUILDDIROBJS)/, $(OBJS))

$(BUILDDIROBJS)/%.o: %.c
	$(CC) -o $@ -c $< $(CFLAGS) $(INCLUDES) -fPIC

.PHONY: clean
