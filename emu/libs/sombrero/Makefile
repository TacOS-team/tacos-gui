OBJS = sombrero.o application.o widget.o window.o container.o bin.o canvas.o timer.o button.o textbox.o label.o checkbox.o color.o

all: $(SOMBRERO)

$(SOMBRERO): $(addprefix $(BUILDDIROBJS)/sombrero_, $(OBJS))
	$(CXX) -o $@ -shared $(addprefix $(BUILDDIROBJS)/sombrero_, $(OBJS)) $(PRONLIB) $(LIBTSOCK) -lc

$(BUILDDIROBJS)/sombrero_%.o: %.cpp
	$(CXX) -o $@ -c $< $(CFLAGS) $(INCLUDES) -fPIC
