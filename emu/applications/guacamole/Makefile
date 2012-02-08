INCLUDES=-I. -I../../include/ -I../../libs
CFLAGS=-Wall -g
BUILDDIR=../../build
BUILDDIRAPP=$(BUILDDIR)/applications

all: $(BUILDDIRAPP)/guacamole

$(BUILDDIRAPP)/guacamole:
	$(CXX) -o $@ guacamole.cpp $(CFLAGS) $(INCLUDES)


clean:
	rm -f $(BUILDDIRAPP)/* 

.PHONY: clean
