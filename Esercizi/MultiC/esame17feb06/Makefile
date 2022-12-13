CFLAGSCONSTRERROR=-ansi -Wpedantic -Wall -D_REENTRANT -D_THREAD_SAFE -D_POSIX_C_SOURCE=200112L
CFLAGS=-ansi -Wpedantic -Wall -D_REENTRANT -D_THREAD_SAFE 

LIBRARIES=-lpthread

all: 	 attacchinisincroni.exe

attacchinisincroni.exe:  attacchinisincroni.o DBGpthread.o 
	gcc -o attacchinisincroni.exe attacchinisincroni.o DBGpthread.o ${LIBRARIES}

attacchinisincroni.o: attacchinisincroni.c DBGpthread.h printerror.h
	gcc -c ${CFLAGS} attacchinisincroni.c

DBGpthread.o: DBGpthread.c printerror.h
	gcc ${CFLAGSCONSTRERROR} -c DBGpthread.c 


.PHONY:	clean

clean:
	-rm -f attacchinisincroni.o DBGpthread.o attacchinisincroni.exe


