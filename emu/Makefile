DEBUG=-D DEBUG
CURRENTDIR=$(realpath .)
INCLUDES= -I. -I$(CURRENTDIR)/include/ -I$(CURRENTDIR)/drivers/include/ -I$(CURRENTDIR)libs/libtacos/include/
CFLAGS=-Wall -g $(DEBUG)

BUILDDIR=$(realpath .)/build
BUILDDIROBJECT=$(BUILDDIR)/objects
BUILDDIRLIBS=$(BUILDDIR)/libs
BUILDDIRAPPS=$(BUILDDIR)/applications

LIBTACOS=$(BUILDDIRLIBS)/libtacos.so
PRONLIB=$(BUILDDIRLIBS)/pronlib.so
LIBSOCK=$(BUILDDIRLIBS)/libsock.so

export LIBTACOS
export PRONLIB
export LIBSOCK

export INCLUDES
export CFLAGS

export BUILDDIROBJECT
export BUILDDIRLIBS
export BUILDDIRAPPS

all: applications

debug:
	$(MAKE) $(MAKEFILE) DEBUG="-D DEBUG"

applications: libs
	$(MAKE) $(MFLAGS) -C applications

drivers:
	$(MAKE) $(MFLAGS) -C drivers

libs: drivers
	$(MAKE) $(MFLAGS) -C libs

$(BUILDDIROBJECT)/%.o: %.c
	$(CC) -o $@ -c $< $(CFLAGS) $(INCLUDES) -fPIC

clean:
	rm -f $(BUILDDIROBJECT)/* $(BUILDDIRLIBS)/* $(BUILDDIRAPPS)/*

.PHONY: applications drivers clean libs
