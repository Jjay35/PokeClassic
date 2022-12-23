all: hw10

hw10:JaysonLeeHw10.cpp
	g++  -g JaysonLeeHw10.cpp -o hw10 -lncurses

clean:
	rm -f 10