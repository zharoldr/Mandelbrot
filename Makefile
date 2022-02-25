
UNAME := $(shell uname)
ifeq ($(UNAME), Darwin)
LIBS = -framework OpenGL -framework GLUT
else
LIBS = -lGL -lGLU -lglut
endif

FLAGS=-Wall -g -O2 -std=c++11
OBJS=mandelbrot.cpp src/*.cpp
INCLUDE=include/

default:
	$(CXX) $(FLAGS) -I$(INCLUDE) $(OBJS) $(LIBS) -o mandelbrot

color:
	$(CXX) $(FLAGS) -I$(INCLUDE) $(OBJS) $(LIBS) -DCOLOR -o mandelbrot

slow:
	$(CXX) $(FLAGS) -I$(INCLUDE) $(OBJS) $(LIBS) -DHIGH_PRECISION -o mandelbrot

movie:
	$(CXX) $(FLAGS) -I$(INCLUDE) $(OBJS) $(LIBS) -DCOLOR -DMOVIE_MODE -o mandelbrot

pic:
	$(CXX) $(FLAGS) -I$(INCLUDE) $(OBJS) $(LIBS) -DCOLOR -DPHOTO_MODE -o mandelbrot

vid:
	make clean
	make movie
	./mandelbrot
	magick mogrify -format png -path pngs imgs/*.ppm
	ffmpeg -r 60 -f image2 -s 512x512 -i pngs/%d.png -vcodec libx264 -crf 25 -pix_fmt yuv420p test.mp4

clean:
	rm -f mandelbrot
	rm -f *.ppm
	rm -f *.png
	rm -f imgs/*
	rm -f pngs/*
