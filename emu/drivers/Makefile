OBJS = vgadrv.o vesadrv.o mousedrv.o

all: $(addprefix $(BUILDDIROBJS)/, $(OBJS))

$(BUILDDIROBJS)/%.o: %.c
	$(CC) -o $@ -c $< $(CFLAGS) $(INCLUDES) -fPIC

.PHONY: clean
