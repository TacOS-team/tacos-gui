SRCS = *.cpp
OBJS = sombrero.o application.o widget.o window.o container.o \
				bin.o canvas.o timer.o button.o textbox.o label.o checkbox.o \
				grid.o doublescrollpane.o scrollpane.o vscrollpane.o hscrollpane.o scrollbar.o vscrollbar.o hscrollbar.o image.o \
				fileinfo.o directory.o radioButtonGroup.o radioButton.o \
        colorpicker.o

all: $(SOMBRERO)

$(SOMBRERO): $(addprefix $(BUILDDIROBJS)/sombrero_, $(OBJS)) $(PRONLIB)
	$(CXX) -o $@ -shared $^ -ljpeg

$(BUILDDIROBJS)/sombrero_%.o: %.cpp
	$(CXX) -o $@ -c $< $(CFLAGS) $(INCLUDES) -fPIC

depend: $(SRCS)
	$(CXX) -MM $(CFLAGS) $(INCLUDES) $^ > .depend.tmp
	sed -E 's#^([^:]+):#$(BUILDDIROBJS)/\1:#' .depend.tmp > .depend
	rm .depend.tmp

-include .depend
