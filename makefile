all:
	g++ -c ga.cpp
	g++ -c neural_net.cpp
	g++ -o main main.cpp ga.o neural_net.o -lX11
