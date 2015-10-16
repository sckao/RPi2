CC=g++
CFLAG= -Wall -ansi

OBJECTS=rpi2.o

all:rpi2.exe

rpi2.exe : rpi2.o
	${CC} $< ${CFLAG} -lbcm2835 -lreadline -o $@

rpi2.o : rpi2.cc rpi2.h
	${CC} -c $< ${CFLAG} -o $@

clean :
	rm -rf *.o rpi2.exe
