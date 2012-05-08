SRCS = *.cpp
OBJS = pron_danny.o

all: $(BUILDDIRAPPS)/pron_danny

$(BUILDDIRAPPS)/pron_danny: $(addprefix $(BUILDDIROBJS)/, $(OBJS))
	$(CXX) -o $@ $^ $(PRONLIB) $(LIBTACOS) $(LIBTSOCK) $(CXXFLAGS) $(INCLUDES) -lm

$(BUILDDIROBJS)/%.o: %.cpp
	$(CXX) -o $@ -c $< $(CFLAGS) $(INCLUDES)

depend: $(SRCS)
	$(CXX) -MM $(CFLAGS) $(INCLUDES) $^ > .depend.tmp
	sed -E 's#^([^:]+):#$(BUILDDIROBJS)/\1:#' .depend.tmp > .depend
	rm .depend.tmp

.PHONY: clean

-include .depend
