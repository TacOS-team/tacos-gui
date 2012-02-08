INCLUDES=-I. -I../include/
CFLAGS=-Wall -g
BUILDDIR=../build
BUILDDIRAPP=$(BUILDDIR)/applications

all:  guacamole \
			$(BUILDDIRAPP)/cube_graph \
			$(BUILDDIRAPP)/cube_graph_mouse \
			$(BUILDDIRAPP)/zen_graph \
			$(BUILDDIRAPP)/pong \
			$(BUILDDIRAPP)/snake_graph \
			$(BUILDDIRAPP)/clock_graph

guacamole:
	$(MAKE) $(MFLAGS) -C guacamole

$(BUILDDIRAPP)/%: %.c
	$(CC) -o $@ $< $(LIBTACOS) $(CFLAGS) $(INCLUDES)

clean:
	rm -f $(BUILDDIRAPP)/* 

.PHONY: clean guacamole
