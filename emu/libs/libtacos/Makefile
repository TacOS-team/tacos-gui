OBJS = vgadrv.o mousedrv.o vesadrv.o kbddrv.o

all: $(LIBTACOS)

$(LIBTACOS): $(BUILDDIROBJS)/libtacos.o
	$(LD) -o $@ -shared $< $(addprefix $(BUILDDIROBJS)/, $(OBJS)) -lSDL -ldl -lc

$(BUILDDIROBJS)/%.o: %.c
	$(CC) -o $@ -c $< $(CFLAGS) $(INCLUDES) -fPIC
