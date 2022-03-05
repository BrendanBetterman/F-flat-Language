CXX = g++
CC = g++
FLAGS = -std=c++0x

all:   fflat 

fflat: fflat.cpp Fparse.o Fscan.o Fcode.o
	$(CXX) $(FLAGS) Fparse.o Fscan.o Fcode.o fflat.cpp -o fflat

fparse.o: fparse.cpp
	$(CXX) $(FLAGS) -c fparse.cpp

fscan.o: fscan.cpp
	$(CXX) $(FLAGS) -c fscan.cpp

fcode.o: fcode.cpp
	$(CXX) $(FLAGS) -c fcode.cpp

clobber:
	rm -f *.o *.exe *.lis *.obj core micro sam macc trace.txt
