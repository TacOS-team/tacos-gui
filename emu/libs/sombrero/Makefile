SRCS = *.cpp
OBJS = sombrero.o application.o widget.o window.o container.o \
				bin.o canvas.o timer.o button.o textbox.o label.o checkbox.o \
				grid.o scrollpane.o scrollbar.o vscrollbar.o

all: $(SOMBRERO)

$(SOMBRERO): $(addprefix $(BUILDDIROBJS)/sombrero_, $(OBJS))
	$(CXX) -o $@ -shared $(addprefix $(BUILDDIROBJS)/sombrero_, $(OBJS)) $(PRONLIB)

$(BUILDDIROBJS)/sombrero_%.o: %.cpp
	$(CXX) -o $@ -c $< $(CFLAGS) $(INCLUDES) -fPIC

depend: $(SRCS)
	$(CXX) -MM $(CFLAGS) $(INCLUDES) $^ > .depend.tmp
	sed -E 's#^([^:]+):#$(BUILDDIROBJS)/\1:#' .depend.tmp > .depend
	rm .depend.tmp

-include .depend
