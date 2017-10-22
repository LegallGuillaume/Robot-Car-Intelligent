GPP = g++ 
TARGET = robot 
SRC = $(wildcard *.cpp) 
COMPILE_FILE = main.cpp sources/manager.cpp sources/bloc.cpp sources/car.cpp sources/chemin.cpp sources/arrive.cpp \
					sources/traitement/calibration.cpp
CFLAGS = -std=c++11
OPENCV = `pkg-config --cflags --libs opencv`
# Toutes les commandes qui suivent peuvent commencer par @ 
# pour empêcher l'affichage : 
# @$(GPP) -o $@ $^ $(CFLAGS) 
# @$(GPP) -c -o $@ $< $(CFLAGS) 
# @rm -f *.o 

$(TARGET) : ${COMPILE_FILE}
	$(GPP) -o $@ $^ $(CFLAGS) $(OPENCV)
 
%.o : %.cpp 
	$(GPP) -c -o $@ $< $(CFLAGS) $(OPENCV)

clean: 
	rm -f *.o ${TARGET}