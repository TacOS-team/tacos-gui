SRCS = *.cpp
OBJS = cuadraw.o

all: $(BUILDDIRAPPS)/cuadraw

$(BUILDDIRAPPS)/cuadraw: $(addprefix $(BUILDDIROBJS)/, $(OBJS)) $(SOMBRERO) $(LIBTACOS) 
	$(CXX) -o $@ $^ $(CXXFLAGS) $(INCLUDES) -lm

$(BUILDDIROBJS)/%.o: %.cpp
	$(CXX) -o $@ -c $< $(CFLAGS) $(INCLUDES)

depend: $(SRCS)
	$(CXX) -MM $(CFLAGS) $(INCLUDES) $^ > .depend.tmp
	sed -E 's#^([^:]+):#$(BUILDDIROBJS)/\1:#' .depend.tmp > .depend
	rm .depend.tmp

.PHONY: clean

-include .depend
