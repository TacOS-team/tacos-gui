
all: $(BUILDDIRAPPS)/guacamole

$(BUILDDIRAPPS)/guacamole: guacamole.cpp
	$(CXX) -o $@ guacamole.cpp $(LIBTACOS) $(CXXFLAGS) $(INCLUDES)

.PHONY: clean
