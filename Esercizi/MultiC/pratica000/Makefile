CFLAGS=-ansi -Wpedantic -Wall -D_REENTRANT -D_THREAD_SAFE -D_POSIX_C_SOURCE=200112L

LIBRARIES=-lpthread

all: 	main.exe

main.exe: main.o DBGpthread.o
	gcc -o main.exe main.o DBGpthread.o ${LIBRARIES}

main.o: main.c DBGpthread.h printerror.h
	gcc -c ${CFLAGS} main.c 

DBGpthread.o: DBGpthread.c printerror.h
	gcc -c ${CFLAGS} DBGpthread.c

.PHONY:	clean

clean:
	-rm -f main.o DBGpthread.o main.exe


