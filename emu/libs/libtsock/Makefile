all: libtsock

libtsock: $(BUILDDIROBJECT)/tsock.o
	$(LD) -o $(LIBTSOCK) -shared $<

$(BUILDDIROBJECT)/%.o: %.c
	$(CC) -o $@ -c $< $(CFLAGS) $(INCLUDES)
