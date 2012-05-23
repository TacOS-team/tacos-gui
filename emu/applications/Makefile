SRCS = *.c *.cpp
APPS = cube_graph cube_graph_mouse zen_graph pong snake_graph pouet clock_graph pron_cube pron_pong pron_cube_mouse pron_snake pron_cube_tuning sombrero_cube sombrero_textbox sombrero_button sombrero_checkbox sombrero_grid jpegviewer sombrero_image sombrero_radioButton

SUBDIRS = guacamole pron pron_danny velo cuadraw

all: $(addprefix $(BUILDDIRAPPS)/, $(APPS))
	@for i in $(SUBDIRS); do \
		printf "\033[1m   >>> [$$i]\033[0m\n"; \
		$(MAKE) $(MFLAGS) -C $$i; \
		if [ $$? = 0 ]; then printf "\033[1m   <<< [$$i] [\033[32mOK\033[0m\033[1m]\033[0m\n"; else printf "\033[31m\033[1m   <<< [$$i] [FAIL]\033[0m\n"; exit 1; fi; \
	done

$(BUILDDIRAPPS)/jpegviewer : jpegviewer.cpp	$(LIBTACOS) $(PRONLIB) 
	$(CXX) -o $@ $^ -ljpeg $(CFLAGS) $(INCLUDES)
	
$(BUILDDIRAPPS)/sombrero_% : sombrero_%.cpp	$(LIBTACOS) $(SOMBRERO)
	$(CXX) -o $@ $^ $(CFLAGS) $(INCLUDES)

$(BUILDDIRAPPS)/%: %.c $(LIBTACOS) 
	$(CC) -o $@ $^ $(CFLAGS) $(INCLUDES)

$(BUILDDIRAPPS)/%: %.cpp $(LIBTACOS) $(PRONLIB) $(LIBCOLOR) 
	$(CXX) -o $@ $^ $(CFLAGS) $(INCLUDES)

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
