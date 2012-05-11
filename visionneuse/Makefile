all: visionneuse

visionneuse: libjpeg
	gcc visionneuse.c -I./jpeg-8d/ -Ljpeg-8d/.libs/ -ljpeg -lSDL -o visionneuse

libjpeg:
	cd jpeg-8d && ./configure && make && cd ..

clean:
	rm visionneuse
