CFLAGS=-ansi -Wpedantic -Wall -D_REENTRANT -D_THREAD_SAFE -D_POSIX_C_SOURCE=200112L -D_DEFAULT_SOURCE
LIBRARIES=-lpthread

all: 		caserma.exe

caserma.exe: 	caserma.o
	gcc ${CFLAGS} -o caserma.exe caserma.o ${LIBRARIES}

caserma.o: 	caserma.c  printerror.h
	gcc -c ${CFLAGS} caserma.c 

.PHONY:		clean

clean:
	-rm -f caserma.o caserma.exe


