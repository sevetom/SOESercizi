CFLAGS=-ansi -Wpedantic -Wall -D_REENTRANT -D_THREAD_SAFE -D_POSIX_C_SOURCE=200112L

LIBRARIES=-lpthread

all: 	alieni.exe

alieni.exe: alieni.o DBGpthread.o
	gcc -o alieni.exe alieni.o DBGpthread.o ${LIBRARIES}

alieni.o: alieni.c DBGpthread.h printerror.h
	gcc -c ${CFLAGS} alieni.c 

DBGpthread.o: DBGpthread.c printerror.h
	gcc -c ${CFLAGS} DBGpthread.c

.PHONY:	clean

clean:
	-rm -f alieni.o DBGpthread.o alieni.exe


