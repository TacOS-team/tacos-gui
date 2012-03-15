OBJS = pron_danny.o

all: $(BUILDDIRAPPS)/pron_danny

$(BUILDDIRAPPS)/pron_danny: $(addprefix $(BUILDDIROBJS)/, $(OBJS))
	$(CXX) -o $@ $^ $(PRONLIB) $(LIBTACOS) $(LIBTSOCK) $(CXXFLAGS) $(INCLUDES) -lm

$(BUILDDIROBJS)/%.o: %.cpp
	$(CC) -o $@ -c $< $(CFLAGS) $(INCLUDES)

.PHONY: clean

