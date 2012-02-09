SUBDIRS = guacamole pron

all:  guacamole pron \
			$(BUILDDIRAPPS)/cube_graph \
			$(BUILDDIRAPPS)/cube_graph_mouse \
			$(BUILDDIRAPPS)/zen_graph \
			$(BUILDDIRAPPS)/pong \
			$(BUILDDIRAPPS)/snake_graph \
			$(BUILDDIRAPPS)/clock_graph
	@for i in $(SUBDIRS); do \
		printf "\033[1m>>> [$$i]\033[0m\n"; \
		$(MAKE) $(MFLAGS) -C $$i; \
		if [ $$? = 0 ]; then printf "\033[1m<<< [$$i] [OK]\033[0m\n"; else printf "\033[31m\033[1m<<< [$$i] [FAIL]\033[0m\n"; exit 1; fi; \
	done

$(BUILDDIRAPPS)/%: %.c
	$(CC) -o $@ $< $(LIBTACOS) $(CFLAGS) $(INCLUDES)

.PHONY: clean guacamole
