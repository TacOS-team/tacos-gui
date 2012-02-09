all: pronlib

pronlib: $(BUILDDIROBJECT)/pronlib.o
	$(LD) -o $(PRONLIB) -shared $< -fc

$(BUILDDIROBJECT)/%.o: %.cpp
	$(CXX) -o $@ -c $< $(CFLAGS) $(INCLUDES) -fPIC
