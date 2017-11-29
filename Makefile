GPP = g++ 
TARGET = robot 
UNITEST = unitest
SRC = $(wildcard *.cpp) 
COMPILE_FILE = main.cpp sources/manager.cpp sources/bloc.cpp sources/car.cpp sources/chemin.cpp sources/arrive.cpp \
					sources/traitement/images.cpp
UNITEST_FILE = unitest.cpp sources/manager.cpp sources/bloc.cpp sources/car.cpp sources/chemin.cpp sources/arrive.cpp \
					sources/traitement/calibration.cpp
CFLAGS = -std=c++11 #-pthread
OPENCV = `pkg-config --cflags --libs opencv`
ARUCO = `pkg-config --cflags --libs aruco`

$(UNITEST) : ${UNITEST_FILE}
	$(GPP) -o $@ $^ $(CFLAGS) $(OPENCV) $(ARUCO)

$(TARGET) : ${COMPILE_FILE}
	$(GPP) -o $@ $^ $(CFLAGS) $(OPENCV) $(ARUCO)
 
%.o : %.cpp 
	$(GPP) -c -o $@ $< $(CFLAGS) $(OPENCV) $(ARUCO)

clean: 
	rm -f *.o ${TARGET} ${UNITEST}