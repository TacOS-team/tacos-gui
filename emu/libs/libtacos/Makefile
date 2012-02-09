OBJS = vga.o mouse.o vesa.o

all: $(LIBTACOS)

$(LIBTACOS): $(BUILDDIROBJS)/libtacos.o
	$(LD) -o $@ -shared $< $(addprefix $(BUILDDIROBJS)/, $(OBJS)) -lSDL -ldl -lc

$(BUILDDIROBJS)/%.o: %.c
	$(CC) -o $@ -c $< $(CFLAGS) $(INCLUDES) -fPIC
