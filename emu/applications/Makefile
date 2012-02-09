APPS = cube_graph cube_graph_mouse zen_graph pong snake_graph pouet clock_graph
SUBDIRS = guacamole pron

all: $(addprefix $(BUILDDIRAPPS)/, $(APPS))
	@for i in $(SUBDIRS); do \
		printf "\033[1m   >>> [$$i]\033[0m\n"; \
		$(MAKE) $(MFLAGS) -C $$i; \
		if [ $$? = 0 ]; then printf "\033[1m   <<< [$$i] [\033[32mOK\033[0m\033[1m]\033[0m\n"; else printf "\033[31m\033[1m   <<< [$$i] [FAIL]\033[0m\n"; exit 1; fi; \
	done

$(BUILDDIRAPPS)/%: %.c
	$(CC) -o $@ $< $(LIBTACOS) $(CFLAGS) $(INCLUDES)

$(BUILDDIRAPPS)/%: %.cpp
	$(CXX) -o $@ $< $(LIBTACOS) $(PRONLIB) $(LIBTSOCK) $(CFLAGS) $(INCLUDES)

.PHONY: clean $(SUBDIRS)
