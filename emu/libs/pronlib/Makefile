all: pronlib

pronlib: $(BUILDDIROBJECT)/pronlib.o
	$(LD) -o $(PRONLIB) -shared $<

$(BUILDDIROBJECT)/%.o: %.c
	$(CC) -o $@ -c $< $(CFLAGS) $(INCLUDES) -fPIC
