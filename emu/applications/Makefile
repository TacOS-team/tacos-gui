
all:  guacamole \
			$(BUILDDIRAPPS)/cube_graph \
			$(BUILDDIRAPPS)/cube_graph_mouse \
			$(BUILDDIRAPPS)/zen_graph \
			$(BUILDDIRAPPS)/pong \
			$(BUILDDIRAPPS)/snake_graph \
			$(BUILDDIRAPPS)/clock_graph

guacamole:
	$(MAKE) $(MFLAGS) -C guacamole

$(BUILDDIRAPPS)/%: %.c
	$(CC) -o $@ $< $(LIBTACOS) $(CFLAGS) $(INCLUDES)

.PHONY: clean guacamole
