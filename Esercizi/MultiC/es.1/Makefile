# Makefile per usare_strerror_r.c
# a causa della presenza di pthread
# occorre compilare col flag -D_REENTRANT
# oppure con  -D_THREAD_SAFE 

# CFLAGSCONDEBUG=-ansi -Wpedantic -Wall -D_REENTRANT  -D_THREAD_SAFE -D_POSIX_C_SOURCE=200112L -ggdb 

CFLAGS=-ansi -Wpedantic -Wall -D_REENTRANT  -D_THREAD_SAFE -D_POSIX_C_SOURCE=200112L
LIBRARIES=-lpthread

all: 	usare_strerror_r.exe 

usare_strerror_r.exe: usare_strerror_r.o
	gcc -o usare_strerror_r.exe usare_strerror_r.o ${LIBRARIES}

usare_strerror_r.o: usare_strerror_r.c
	gcc ${CFLAGS} -c usare_strerror_r.c


.PHONY:	clean

clean:
	rm -f usare_strerror_r.o usare_strerror_r.exe 

