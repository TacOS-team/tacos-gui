all: $(BUILDDIRAPPS)/pron

$(BUILDDIRAPPS)/pron: pron.cpp
	$(CXX) -o $@ $< $(LIBTACOS) $(LIBTSOCK) $(CXXFLAGS) $(INCLUDES)
	
.PHONY: clean
