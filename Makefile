GPP=gcc
LINKER=gcc-ar
# CXXFLAGS= -Wall -fopenmp -flto -fno-fat-lto-objects -std=c++11
CXXFLAGS= -Wall -fopenmp -O3 -march=native -std=c++11
LDFLAGS=  rcs
OBJS=build/libENND.a
LIBS_OBJS= build/Nodo.o build/timeManager.o build/flagVector.o build/ENND.o

all: ${OBJS}

clean:
	rm -rf ${OBJS} ${LIBS_OBJS}

${OBJS}: ${LIBS_OBJS}
	${LINKER} ${LDFLAGS} ${OBJS} ${LIBS_OBJS}

build/Nodo.o: Nodo.cpp Nodo.h build
	${GPP} ${CXXFLAGS} -fPIC -c Nodo.cpp -o $@

build/timeManager.o: timeManager.cpp timeManager.h build
	${GPP} ${CXXFLAGS} -fPIC -c timeManager.cpp -o $@

build/flagVector.o: flagVector.cpp flagVector.h build
	${GPP} ${CXXFLAGS} -fPIC -c flagVector.cpp -o $@

build/ENND.o: ENND.cpp ENND.h build
	${GPP} ${CXXFLAGS} -fPIC -c ENND.cpp -o $@

build:
	mkdir build

