CXX = g++
CC = g++
FLAGS = -std=c++0x

all:   fflat 

fflat: fflat.cpp FParse.o FScan.o Fcode.o
	$(CXX) $(FLAGS) FParse.o FScan.o FCodeGen.o fflat.cpp -o fflat

FParse.o: FParse.cpp
	$(CXX) $(FLAGS) -c FParse.cpp

FScan.o: FScan.cpp
	$(CXX) $(FLAGS) -c FScan.cpp

FCodeGen.o: FCodeGen.cpp
	$(CXX) $(FLAGS) -c FCodeGen.cpp

clobber:
	rm -f *.o *.exe *.lis *.obj core micro sam macc trace.txt
