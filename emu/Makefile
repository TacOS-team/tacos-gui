# Colorize outputs
HASCOLOR = $(shell if test `which colorgcc`; then echo true; else echo false; fi)
ifneq ($(HASCOLOR), true)
	export CC=@printf "\033[34m   CC\t$$@\033[0m\n" && gcc
else
	export CC=@printf "\033[34m   CC\t$$@\033[0m\n" && colorgcc
endif

export CXX=@printf "\033[34m   CPPC\t$$@\033[0m\n" && g++

export AR=@printf "\033[34m   AR\t$$@\033[0m\n" && ar

export LD=@printf "\033[35m   LD\t$$@\033[0m\n" && ld

# Compilation flags
DEBUG=-DDEBUG
CURRENTDIR=$(realpath .)
export INCLUDES= -I. -I$(CURRENTDIR)/include/ -I$(CURRENTDIR)/drivers/include/ -I$(CURRENTDIR)/libs/libtacos/ -I$(CURRENTDIR)/libs/pronlib/ -I$(CURRENTDIR)/libs/libtsock/ -I$(CURRENTDIR)/libs/sombrero/
export CFLAGS=-W -Wall -g $(DEBUG)
export CXXFLAGS=-W -Wall -g $(DEBUG)
export MFLAGS=-s

# Build paths
BUILDDIR=$(realpath .)/build
export BUILDDIROBJS=$(BUILDDIR)/objects
export BUILDDIRLIBS=$(BUILDDIR)/libs
export BUILDDIRAPPS=$(BUILDDIR)/applications

# Path to libraries
export LIBTACOS=$(BUILDDIRLIBS)/libtacos.so
export PRONLIB=$(BUILDDIRLIBS)/pronlib.so
export LIBTSOCK=$(BUILDDIRLIBS)/libtsock.so
export SOMBRERO=$(BUILDDIRLIBS)/sombrero.so

SUBDIRS = drivers libs applications

# Make all subdirectories
all:
	@for i in $(SUBDIRS); do \
		printf "\033[1m>>> [$$i]\033[0m\n"; \
		$(MAKE) $(MFLAGS) -C $$i; \
		if [ $$? = 0 ]; then printf "\033[1m<<< [$$i] [\033[32mOK\033[0m\033[1m]\033[0m\n"; else printf "\033[31m\033[1m<<< [$$i] [FAIL]\033[0m\n"; exit 1; fi; \
	done

debug:
	$(MAKE) $(MAKEFILE) DEBUG="-DDEBUG"

clean:
	rm -f $(BUILDDIROBJS)/* $(BUILDDIRLIBS)/* $(BUILDDIRAPPS)/*

.PHONY: clean $(SUBDIRS)
