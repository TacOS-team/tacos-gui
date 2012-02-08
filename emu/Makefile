INCLUDES= -I./include/
CFLAGS=-Wall -g
BUILDDIR=$(realpath .)/build
BUILDDIROBJECT=$(BUILDDIR)/objects
BUILDDIRLIBS=$(BUILDDIR)/objects
LIBTACOS=$(BUILDDIRLIBS)/libtacos.so
export LIBTACOS

all: applications

applications: libtacos
	$(MAKE) $(MFLAGS) -C applications

drivers:
	$(MAKE) $(MFLAGS) -C drivers

libtacos: $(BUILDDIROBJECT)/libtacos.o drivers
	$(LD) -o $(LIBTACOS) -shared $(BUILDDIROBJECT)/vga.o $(BUILDDIROBJECT)/mouse.o $(BUILDDIROBJECT)/vesa.o $(BUILDDIROBJECT)/libtacos.o -lSDL -ldl

$(BUILDDIROBJECT)/%.o: %.c
	$(CC) -o $@ -c $< $(CFLAGS) $(INCLUDES) -fPIC

clean:
	$(MAKE) $@ $(MFLAGS) -C applications
	$(MAKE) $@ $(MFLAGS) -C drivers
	rm -f $(BUILDDIROBJECT)/* libtacos.so

.PHONY: applications drivers clean
