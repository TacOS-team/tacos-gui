all: $(PRONLIB)

$(PRONLIB): $(BUILDDIROBJS)/pronlib.o
	$(LD) -o $@ -shared $< -lc

$(BUILDDIROBJS)/%.o: %.cpp
	$(CXX) -o $@ -c $< $(CFLAGS) $(INCLUDES) -fPIC
