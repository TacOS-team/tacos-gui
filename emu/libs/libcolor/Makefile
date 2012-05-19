SRCS = *.cpp
OBJS = color.o

all: $(LIBCOLOR)

$(LIBCOLOR): $(addprefix $(BUILDDIROBJS)/libcolor_, $(OBJS))
	$(CXX) -o $@ -shared $^

$(BUILDDIROBJS)/libcolor_%.o: %.cpp
	$(CXX) -o $@ -c $< $(CFLAGS) $(INCLUDES) -fPIC

depend: $(SRCS)
	$(CXX) -MM $(CFLAGS) $(INCLUDES) $^ > .depend.tmp
	sed -E 's#^([^:]+):#$(BUILDDIROBJS)/\1:#' .depend.tmp > .depend
	rm .depend.tmp

-include .depend
