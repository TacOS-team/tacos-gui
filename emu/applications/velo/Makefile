SRCS = *.cpp
OBJS = velo.o

all: $(BUILDDIRAPPS)/velo

$(BUILDDIRAPPS)/velo: $(addprefix $(BUILDDIROBJS)/, $(OBJS))
	$(CXX) -o $@ $^ $(SOMBRERO) $(LIBTACOS) $(CXXFLAGS) $(INCLUDES) -lm

$(BUILDDIROBJS)/%.o: %.cpp
	$(CXX) -o $@ -c $< $(CFLAGS) $(INCLUDES)

depend: $(SRCS)
	$(CXX) -MM $(CFLAGS) $(INCLUDES) $^ > .depend.tmp
	sed -E 's#^([^:]+):#$(BUILDDIROBJS)/\1:#' .depend.tmp > .depend
	rm .depend.tmp

.PHONY: clean

-include .depend
