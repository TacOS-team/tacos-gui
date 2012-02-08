
all:  $(BUILDDIROBJECT)/vga.o \
			$(BUILDDIROBJECT)/vesa.o \
			$(BUILDDIROBJECT)/mouse.o

$(BUILDDIROBJECT)/%.o: %.c
	$(CC) -o $@ -c $< $(CFLAGS) $(INCLUDES) -fPIC

.PHONY: clean
