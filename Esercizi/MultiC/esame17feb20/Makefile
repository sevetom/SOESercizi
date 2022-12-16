CFLAGS=-ansi -Wpedantic -Wall -D_REENTRANT -D_THREAD_SAFE -D_POSIX_C_SOURCE=200112L

LIBRARIES=-lpthread

all: 	conigli.exe

conigli.exe: conigli.o DBGpthread.o
	gcc -o conigli.exe conigli.o DBGpthread.o ${LIBRARIES}

conigli.o: conigli.c DBGpthread.h printerror.h
	gcc -c ${CFLAGS} conigli.c 

DBGpthread.o: DBGpthread.c printerror.h
	gcc -c ${CFLAGS} DBGpthread.c

.PHONY:	clean

clean:
	-rm -f conigli.o DBGpthread.o conigli.exe


