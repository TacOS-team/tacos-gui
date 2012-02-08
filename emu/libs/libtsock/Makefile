
all: libtsock

libtsock: $(BUILDDIROBJECT)/tsock.o
	$(LD) -o $(LIBSOCK) -shared $^

$(BUILDDIROBJECT)/%.o: %.c
	$(CC) -o $@ -c $< $(CFLAGS) $(INCLUDES) -fPIC
