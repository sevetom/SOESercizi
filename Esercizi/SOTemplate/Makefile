CFLAGS=-ansi -Wpedantic -Wall -D_REENTRANT -D_THREAD_SAFE -D_POSIX_C_SOURCE=200112L -D_DEFAULT_SOURCE
LIBRARIES=-lpthread

all: 		tempC.exe

tempC.exe: 	tempC.o
	gcc ${CFLAGS} -o tempC.exe tempC.o ${LIBRARIES}

tempC.o: 	tempC.c  printerror.h
	gcc -c ${CFLAGS} temp.c 

.PHONY:		clean

clean:
	-rm -f tempC.o tempC.exe


