OBJS = pron.o window.o screen.o

all: $(BUILDDIRAPPS)/pron

$(BUILDDIRAPPS)/pron: $(addprefix $(BUILDDIROBJS)/, $(OBJS))
	$(CXX) -o $@ $^ $(LIBTACOS) $(LIBTSOCK) $(CXXFLAGS) $(INCLUDES)

$(BUILDDIROBJS)/%.o: %.cpp
	$(CC) -o $@ -c $< $(CFLAGS) $(INCLUDES)
	
.PHONY: clean
