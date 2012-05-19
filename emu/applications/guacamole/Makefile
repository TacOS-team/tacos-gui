SRCS = *.cpp
OBJS = guacamole.o gwindows_manager.o gwindow.o

all: $(BUILDDIRAPPS)/guacamole

$(BUILDDIRAPPS)/guacamole: $(addprefix $(BUILDDIROBJS)/, $(OBJS)) $(LIBTACOS) $(PRONLIB) $(LIBCOLOR)
	$(CXX) -o $@ $^ $(CXXFLAGS) $(INCLUDES)

$(BUILDDIROBJS)/%.o: %.cpp
	$(CXX) -o $@ -c $< $(CXXFLAGS) $(INCLUDES)

depend: $(SRCS)
	$(CXX) -MM $(CFLAGS) $(INCLUDES) $^ > .depend.tmp
	sed -E 's#^([^:]+):#$(BUILDDIROBJS)/\1:#' .depend.tmp > .depend
	rm .depend.tmp
	
.PHONY: clean

-include .depend
