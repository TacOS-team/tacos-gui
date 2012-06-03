SRCS = *.cpp
OBJS = bmp.o

all: $(LIBBMP)

$(LIBBMP): $(addprefix $(BUILDDIROBJS)/libbmp_, $(OBJS))
	$(CXX) -o $@ -shared $^

$(BUILDDIROBJS)/libbmp_%.o: %.cpp
	$(CXX) -o $@ -c $< $(CFLAGS) $(INCLUDES) -fPIC

depend: $(SRCS)
	$(CXX) -MM $(CFLAGS) $(INCLUDES) $^ > .depend.tmp
	sed -E 's#^([^:]+):#$(BUILDDIROBJS)/\1:#' .depend.tmp > .depend
	rm .depend.tmp

-include .depend
