CC =g++
CFLAGS =-lGL\
		 -lGLU\
		 -lglut\
		 -std=c++11\

all: trabalhocg

trabalhocg: *.o
	$(CC) -o  trabalhocg *.cpp $(CFLAGS)
clean:
	rm -rf *.o trabalhocg
