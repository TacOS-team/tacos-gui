INCLUDES=-I. -I../../include/ -I../../libs
CFLAGS=-Wall -g

all: guacamole

guacamole:
	$(CXX) -o ../../build/$@ guacamole.cpp $(LIBTACOS) $(CFLAGS) $(INCLUDES)


clean:
	rm -f ../../build/* 

.PHONY: clean
