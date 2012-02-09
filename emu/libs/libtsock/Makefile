all: libtsock

libtsock: $(BUILDDIROBJECT)/tsock.o
	$(LD) -o $(LIBTSOCK) -shared $< -lc

$(BUILDDIROBJECT)/%.o: %.c
	$(CC) -o $@ -c $< $(CFLAGS) $(INCLUDES) -fPIC
