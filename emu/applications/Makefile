INCLUDES=-I. -I../include/
CFLAGS=-Wall -g

all: guacamole cube_graph cube_graph_mouse zen_graph pong snake_graph clock_graph

guacamole:
	$(MAKE) $(MFLAGS) -C guacamole

%: %.c
	$(CC) -o ../build/$@ $< $(LIBTACOS) $(CFLAGS) $(INCLUDES)

clean:
	rm -f ../build/* 

.PHONY: clean guacamole
