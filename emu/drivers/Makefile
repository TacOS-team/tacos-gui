OBJS = vga.o vesa.o mouse.o

all: $(addprefix $(BUILDDIROBJS)/, $(OBJS))

$(BUILDDIROBJS)/%.o: %.c
	$(CC) -o $@ -c $< $(CFLAGS) $(INCLUDES) -fPIC

.PHONY: clean
