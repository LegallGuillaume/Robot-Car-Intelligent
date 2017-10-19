
COMPILE_FILE = main.cpp sources/manager.cpp sources/bloc.cpp sources/car.cpp sources/chemin.cpp

all:
	g++ -g -std=c++11 -o robot ${COMPILE_FILE}

clean:
	rm robot test