SRCS = *.c *.cpp
APPS = cube_graph cube_graph_mouse zen_graph pong snake_graph pouet clock_graph pron_cube pron_pong pron_cube_mouse pron_snake pron_cube_tuning sombrero_cube sombrero_textbox sombrero_button sombrero_checkbox

SUBDIRS = guacamole pron pron_danny

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
		
$(BUILDDIRAPPS)/sombrero_cube : sombrero_cube.cpp	
	$(CXX) -o $@ $< $(LIBTACOS) $(SOMBRERO) $(CFLAGS) $(INCLUDES)

$(BUILDDIRAPPS)/sombrero_button : sombrero_button.cpp	
	$(CXX) -o $@ $< $(LIBTACOS) $(SOMBRERO) $(CFLAGS) $(INCLUDES)

$(BUILDDIRAPPS)/sombrero_textbox : sombrero_textbox.cpp	
	$(CXX) -o $@ $< $(LIBTACOS) $(SOMBRERO) $(CFLAGS) $(INCLUDES)
	
$(BUILDDIRAPPS)/sombrero_checkbox : sombrero_checkbox.cpp	
	$(CXX) -o $@ $< $(LIBTACOS) $(SOMBRERO) $(CFLAGS) $(INCLUDES)

depend: $(SRCS)
	$(CXX) -MM $(CFLAGS) $(INCLUDES) $^ > .depend.tmp
	sed -E 's#^([^:]+):#$(BUILDDIROBJS)/\1:#' .depend.tmp > .depend
	rm .depend.tmp
	@for i in $(SUBDIRS); do \
		printf "\033[1m>>> [$$i]\033[0m\n"; \
		$(MAKE) -s -C $$i depend; \
		printf "\033[1m<<< [$$i]\033[0m\n"; \
	done

.PHONY: clean $(SUBDIRS)

-include .depend
