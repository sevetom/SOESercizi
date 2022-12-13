CFLAGS=-ansi -Wpedantic -Wall -D_REENTRANT -D_THREAD_SAFE -D_POSIX_C_SOURCE=200112L -D_DEFAULT_SOURCE
LIBRARIES=-lpthread

all: 		zambesi.exe

zambesi.exe: 	zambesi.o
	gcc ${CFLAGS} -o zambesi.exe zambesi.o ${LIBRARIES}

zambesi.o: 		zambesi.c  printerror.h
	gcc -c ${CFLAGS} zambesi.c 

.PHONY:		clean

clean:
	-rm -f zambesi.o zambesi.exe


