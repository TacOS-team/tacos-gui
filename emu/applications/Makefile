SUBDIRS = guacamole pron

all:  $(BUILDDIRAPPS)/cube_graph \
			$(BUILDDIRAPPS)/cube_graph_mouse \
			$(BUILDDIRAPPS)/zen_graph \
			$(BUILDDIRAPPS)/pong \
			$(BUILDDIRAPPS)/snake_graph \
			$(BUILDDIRAPPS)/pouet \
			$(BUILDDIRAPPS)/clock_graph
	@for i in $(SUBDIRS); do \
		printf "\033[1m>>> [$$i]\033[0m\n"; \
		$(MAKE) $(MFLAGS) -C $$i; \
		if [ $$? = 0 ]; then printf "\033[1m<<< [$$i] [\033[32mOK\033[30m]\033[0m\n"; else printf "\033[31m\033[1m<<< [$$i] [FAIL]\033[0m\n"; exit 1; fi; \
	done

$(BUILDDIRAPPS)/%: %.c
	$(CC) -o $@ $< $(LIBTACOS) $(PRONLIB) $(LIBTSOCK) $(CFLAGS) $(INCLUDES)

$(BUILDDIRAPPS)/%: %.cpp
	$(CXX) -o $@ $< $(LIBTACOS) $(PRONLIB) $(LIBTSOCK) $(CFLAGS) $(INCLUDES)

.PHONY: clean guacamole
