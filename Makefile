cppall:
	g++ -c -Wall -Wvla -Wextra -std=c++14 FractalDrawer.cpp
	g++ -c -Wall -Wvla -Wextra -std=c++14 Fractal.cpp
	g++ -Wall -Wvla -Wextra FractalDrawer.o Fractal.o -lm -o fractal-drawer