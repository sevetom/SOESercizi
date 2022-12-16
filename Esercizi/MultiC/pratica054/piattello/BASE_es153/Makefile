CFLAGS=-ansi -Wpedantic -Wall -D_REENTRANT -D_THREAD_SAFE -D_POSIX_C_SOURCE=200112L

LIBRARIES=-lpthread

all: 	piattello.exe

piattello.exe: piattello.o DBGpthread.o
	gcc -o piattello.exe piattello.o DBGpthread.o ${LIBRARIES}

piattello.o: piattello.c DBGpthread.h printerror.h
	gcc -c ${CFLAGS} piattello.c 

DBGpthread.o: DBGpthread.c printerror.h
	gcc -c ${CFLAGS} DBGpthread.c

.PHONY:	clean

clean:
	-rm -f piattello.o DBGpthread.o piattello.exe


