SUBDIRS = libtacos libtsock pronlib

all:
	@for i in $(SUBDIRS); do \
		printf "\033[1m   >>> [$$i]\033[0m\n"; \
		$(MAKE) $(MFLAGS) -C $$i; \
		if [ $$? = 0 ]; then printf "\033[1m   <<< [$$i] [\033[32mOK\033[0m\033[1m]\033[0m\n"; else printf "\033[31m\033[1m   <<< [$$i] [FAIL]\033[0m\n"; exit 1; fi; \
	done

.PHONY: libtacos libtsock pronlib
