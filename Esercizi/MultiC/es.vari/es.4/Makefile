CFLAGS=-ansi -Wpedantic -Wall -D_REENTRANT -D_THREAD_SAFE -D_POSIX_C_SOURCE=200112L -D_DEFAULT_SOURCE
LIBRARIES=-lpthread

all: 		pizzeria.exe

pizzeria.exe: 	pizzeria.o
	gcc ${CFLAGS} -o pizzeria.exe pizzeria.o ${LIBRARIES}

pizzeria.o: 	pizzeria.c  printerror.h
	gcc -c ${CFLAGS} pizzeria.c 

.PHONY:		clean

clean:
	-rm -f pizzeria.o pizzeria.exe


