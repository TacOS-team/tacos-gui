INCLUDES=-I. -I../include/
CFLAGS=-Wall -g

all: cube_graph cube_graph_mouse zen_graph pong snake_graph clock_graph

%: %.c
	$(CC) -o $@ $< $(CFLAGS) $(INCLUDES)

clean:
	rm -f cube_graph cube_graph_mouse zen_graph pong snake_graph clock_graph

.PHONY: clean
