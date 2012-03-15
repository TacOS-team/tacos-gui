OBJS = guacamole.o gwindows_manager.o gwindow.o

all: $(BUILDDIRAPPS)/guacamole

$(BUILDDIRAPPS)/guacamole: $(addprefix $(BUILDDIROBJS)/, $(OBJS))
	$(CXX) -o $@ $^ $(LIBTACOS) $(PRONLIB) $(LIBTSOCK) $(CXXFLAGS) $(INCLUDES)

$(BUILDDIROBJS)/%.o: %.cpp
	$(CXX) -o $@ -c $< $(CXXFLAGS) $(INCLUDES)
	
.PHONY: clean
