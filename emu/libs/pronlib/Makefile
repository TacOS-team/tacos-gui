all: $(PRONLIB)

$(PRONLIB): $(BUILDDIROBJS)/pronlib.o
	$(LD) -o $@ -shared $< -fc

$(BUILDDIROBJS)/%.o: %.cpp
	$(CXX) -o $@ -c $< $(CFLAGS) $(INCLUDES) -fPIC
