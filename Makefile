CXX=g++
CFALG=-g -Wall
target=main
main:pre.o main.o
	${CXX} -o $@ $^
pre.o:pre.cpp pre.h
	${CXX} -c $<
main.o:main.cpp pre.h
	${CXX} -c $<
	
.PHONY:clean

clean:
	-rm *.o main
