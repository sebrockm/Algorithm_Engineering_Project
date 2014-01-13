test: default
	./test >> data.dat

default: test.cpp VEB.hpp Trees.hpp
	g++-4.7 -m64 -O3 -Wall -std=c++11 -o test test.cpp -DH=$(N)

