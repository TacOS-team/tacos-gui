SRCS = *.cpp

all: $(PRONLIB)

$(PRONLIB): $(BUILDDIROBJS)/pronlib.o $(LIBTSOCK) $(LIBCOLOR)
	$(CXX) -o $@ -shared $^

$(BUILDDIROBJS)/%.o: %.cpp
	$(CXX) -o $@ -c $< $(CFLAGS) $(INCLUDES) -fPIC

depend: $(SRCS)
	$(CXX) -MM $(CFLAGS) $(INCLUDES) $^ > .depend.tmp
	sed -E 's#^([^:]+):#$(BUILDDIROBJS)/\1:#' .depend.tmp > .depend
	rm .depend.tmp

-include .depend
