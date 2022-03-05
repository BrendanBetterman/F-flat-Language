CXX = g++
CC = g++
FLAGS = -std=c++0x

all:   fflat 

fflat: fflat.cpp Fparse.o Fscan.o Fcode.o
	$(CXX) $(FLAGS) Fparse.o Fscan.o FCodeGen.o fflat.cpp -o fflat

Fparse.o: FParse.cpp
	$(CXX) $(FLAGS) -c FParse.cpp

Fscan.o: FScan.cpp
	$(CXX) $(FLAGS) -c FScan.cpp

FCodeGen.o: FCodeGen.cpp
	$(CXX) $(FLAGS) -c FCodeGen.cpp

clobber:
	rm -f *.o *.exe *.lis *.obj core micro sam macc trace.txt
