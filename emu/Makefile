HASCOLOR = $(shell if test `which colorgcc`; then echo true; else echo false; fi)
ifneq ($(HASCOLOR),true)
	export CC=@printf "\033[34m   CC   $$@\033[0m\n" && gcc
else
	export CC=@printf "\033[34m   CC   $$@\033[0m\n" && colorgcc
endif

export CXX=@printf "\033[34m   CPPC   $$@\033[0m\n" && g++


export AR=@printf "\033[32m   AR   $$@\033[0m\n" && ar

export LD=@printf "\033[31m   LD   $$@\033[0m\n" && ld

DEBUG=-DDEBUG
CURRENTDIR=$(realpath .)
INCLUDES= -I. -I$(CURRENTDIR)/include/ -I$(CURRENTDIR)/drivers/include/ -I$(CURRENTDIR)/libs/libtacos/include/ -I$(CURRENTDIR)/libs/pronlib/ -I$(CURRENTDIR)/libs/libtsock/
CFLAGS=-Wall -g $(DEBUG)
CXXFLAGS=-Wall -g $(DEBUG)

export MFLAGS=-s

BUILDDIR=$(realpath .)/build
BUILDDIROBJECT=$(BUILDDIR)/objects
BUILDDIRLIBS=$(BUILDDIR)/libs
BUILDDIRAPPS=$(BUILDDIR)/applications

LIBTACOS=$(BUILDDIRLIBS)/libtacos.so
PRONLIB=$(BUILDDIRLIBS)/pronlib.so
LIBTSOCK=$(BUILDDIRLIBS)/libtsock.so

export LIBTACOS
export PRONLIB
export LIBTSOCK

export INCLUDES
export CFLAGS
export CXXFLAGS

export BUILDDIROBJECT
export BUILDDIRLIBS
export BUILDDIRAPPS

SUBDIRS = drivers libs applications

all:
	@for i in $(SUBDIRS); do \
		printf "\033[1m>>> [$$i]\033[0m\n"; \
		$(MAKE) $(MFLAGS) -C $$i; \
		if [ $$? = 0 ]; then printf "\033[1m<<< [$$i] [\033[32mOK\033[0m]\033[0m\n"; else printf "\033[31m\033[1m<<< [$$i] [FAIL]\033[0m\n"; exit 1; fi; \
	done

debug:
	$(MAKE) $(MAKEFILE) DEBUG="-DDEBUG"


clean:
	rm -f $(BUILDDIROBJECT)/* $(BUILDDIRLIBS)/* $(BUILDDIRAPPS)/*

.PHONY: applications drivers clean libs
