CFLAGS=-ansi -Wpedantic -Wall -D_REENTRANT -D_THREAD_SAFE -D_POSIX_C_SOURCE=200112L -D_DEFAULT_SOURCE
LIBRARIES=-lpthread

all: 		prelievoModuli.exe

prelievoModuli.exe: 	prelievoModuli.o
	gcc ${CFLAGS} -o prelievoModuli.exe prelievoModuli.o ${LIBRARIES}

tprelievoModuli.o: 	prelievoModuli.c  printerror.h
	gcc -c ${CFLAGS} prelievoModuli.c 

.PHONY:		clean

clean:
	-rm -f prelievoModuli.o prelievoModuli.exe


