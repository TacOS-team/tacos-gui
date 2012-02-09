all: pronlib

pronlib: $(BUILDDIROBJECT)/pronlib.o
	$(LD) -o $(PRONLIB) -shared $< -fc

$(BUILDDIROBJECT)/%.o: %.c
	$(CC) -o $@ -c $< $(CFLAGS) $(INCLUDES) -fPIC
