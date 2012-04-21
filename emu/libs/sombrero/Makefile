OBJS = sombrero.o application.o

all: $(SOMBRERO)

$(SOMBRERO): $(addprefix $(BUILDDIROBJS)/, $(OBJS))
	$(LD) -o $@ -shared $(addprefix $(BUILDDIROBJS)/, $(OBJS)) $(PRONLIB) $(LIBTSOCK) -lc

$(BUILDDIROBJS)/%.o: %.cpp
	$(CXX) -o $@ -c $< $(CFLAGS) $(INCLUDES) -fPIC
