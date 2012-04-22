OBJS = sombrero.o application.o widget.o window.o

all: $(SOMBRERO)

$(SOMBRERO): $(addprefix $(BUILDDIROBJS)/sombrero_, $(OBJS))
	$(LD) -o $@ -shared $(addprefix $(BUILDDIROBJS)/sombrero_, $(OBJS)) $(PRONLIB) $(LIBTSOCK) -lc

$(BUILDDIROBJS)/sombrero_%.o: %.cpp
	$(CXX) -o $@ -c $< $(CFLAGS) $(INCLUDES) -fPIC
