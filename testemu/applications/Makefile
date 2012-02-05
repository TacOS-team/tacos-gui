INCLUDES=-I. -I../include/
CFLAGS=-Wall -g

all: cube_graph cube_graph_mouse zen_graph pong

%: %.c
	$(CC) -o $@ $< $(CFLAGS) $(INCLUDES)
