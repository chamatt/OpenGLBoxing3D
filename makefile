CC =g++
CFLAGS =-lGL\
		 -lGLU\
		 -lglut\
		 -std=c++11\
		 -w

all: trabalhocg

trabalhocg: *.o
	$(CC) -o  trabalhocg *.cpp *c $(CFLAGS)
clean:
	rm -rf *.o trabalhocg
