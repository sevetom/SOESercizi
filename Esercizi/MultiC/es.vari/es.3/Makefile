FLAGS=-ansi -Wall -Wpedantic -Werror -D_REENTRANT -D_THREAD_SAFE -D_POSIX_C_SOURCE=200112L -D_DEFAULT_SOURCE
LIB=-lpthread

all:		ponte.exe

ponte.exe:	ponte.o
	gcc ${FLAGS} -o ponte.exe ponte.o ${LIB}

ponte.o:	ponte.c
	gcc -c ${FLAGS} ponte.c

.PHONY:		clean

clean:
	-rm -f *.o *.exe:
