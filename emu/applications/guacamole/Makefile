all: $(BUILDDIRAPPS)/guacamole

$(BUILDDIRAPPS)/guacamole: guacamole.cpp
	$(CXX) -o $@ guacamole.cpp $(LIBTACOS) $(PRONLIB) $(LIBTSOCK) $(CXXFLAGS) $(INCLUDES)

.PHONY: clean
