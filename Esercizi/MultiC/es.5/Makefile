CFLAGS=-ansi -Wpedantic -Wall -D_REENTRANT -D_THREAD_SAFE -D_POSIX_C_SOURCE=200112L -D_DEFAULT_SOURCE

LIBRARIES=-lpthread

all: 	usoContemporaneo.exe

usoContemporaneo.exe: usoContemporaneo.o DBGpthread.o
	gcc -o usoContemporaneo.exe usoContemporaneo.o DBGpthread.o ${LIBRARIES}

usoContemporaneo.o: usoContemporaneo.c DBGpthread.h printerror.h
	gcc -c ${CFLAGS} usoContemporaneo.c 

DBGpthread.o: DBGpthread.c printerror.h
	gcc -c ${CFLAGS} DBGpthread.c

.PHONY:	clean

clean:
	-rm -f usoContemporaneo.o DBGpthread.o usoContemporaneo.exe


