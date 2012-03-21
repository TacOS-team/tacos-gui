OBJS = pron.o gc.o screen.o mouse.o keyboard.o window.o client.o clip_zone.o clip_rect.o drawable.o

all: $(BUILDDIRAPPS)/pron

$(BUILDDIRAPPS)/pron: $(addprefix $(BUILDDIROBJS)/, $(OBJS))
	$(CXX) -o $@ $^ $(LIBTACOS) $(LIBTSOCK) $(CXXFLAGS) $(INCLUDES)

$(BUILDDIROBJS)/%.o: %.cpp
	$(CXX) -o $@ -c $< $(CXXFLAGS) $(INCLUDES)
	
.PHONY: clean
