all: libtacos

libtacos: $(BUILDDIROBJECT)/libtacos.o
	$(LD) -o $(LIBTACOS) -shared $(BUILDDIROBJECT)/vga.o $(BUILDDIROBJECT)/mouse.o $(BUILDDIROBJECT)/vesa.o $< -lSDL -ldl

$(BUILDDIROBJECT)/%.o: %.c
	$(CC) -o $@ -c $< $(CFLAGS) $(INCLUDES) -fPIC
