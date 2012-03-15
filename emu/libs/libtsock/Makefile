all: $(LIBTSOCK)

$(LIBTSOCK): $(BUILDDIROBJS)/tsock.o
	$(LD) -o $@ -shared $< -lc

$(BUILDDIROBJS)/%.o: %.c
	$(CC) -o $@ -c $< $(CFLAGS) $(INCLUDES) -fPIC
