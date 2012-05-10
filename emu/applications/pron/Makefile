SRCS = *.cpp
OBJS = pron.o gc.o screen.o mouse.o keyboard.o window.o client.o clip_zone.o clip_rect.o drawable.o windowsTree.o pixmap.o font.o font_loader.o

all: $(BUILDDIRAPPS)/pron

$(BUILDDIRAPPS)/pron: $(addprefix $(BUILDDIROBJS)/, $(OBJS))
	$(CXX) -o $@ $^ $(LIBTACOS) $(LIBTSOCK) $(LIBCOLOR) $(CXXFLAGS) $(INCLUDES)

$(BUILDDIROBJS)/%.o: %.cpp
	$(CXX) -o $@ -c $< $(CXXFLAGS) $(INCLUDES)

depend: $(SRCS)
	$(CXX) -MM $(CFLAGS) $(INCLUDES) $^ > .depend.tmp
	sed -E 's#^([^:]+):#$(BUILDDIROBJS)/\1:#' .depend.tmp > .depend
	rm .depend.tmp
	
.PHONY: clean

-include .depend
