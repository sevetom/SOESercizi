/* file:  piattello.c */

#ifndef _THREAD_SAFE
	#define _THREAD_SAFE
#endif
#ifndef _POSIX_C_SOURCE
	#define _POSIX_C_SOURCE 200112L
#endif


/* messo prima perche' contiene define _POSIX_C_SOURCE */
#include "printerror.h"

#include <unistd.h>   /* exit() etc */
#include <stdlib.h>     /* random  srandom */
#include <stdio.h>
#include <string.h>     /* per strerror_r  and  memset */
#include <sys/types.h>
#include <signal.h>
#include <sys/time.h> /*gettimeofday() struct timeval timeval{} for select()*/
#include <time.h> /* timespec{} for pselect() */
#include <limits.h> /* for OPEN_MAX */
#include <errno.h>
#include <assert.h>
#include <stdint.h>     /* uint64_t intptr_t */
#include <inttypes.h>   /* per PRIiPTR */
#include <fcntl.h>
#include <sys/stat.h>
#include <pthread.h>

#include "DBGpthread.h"

#define N 5

/* dati da proteggere - AGGIUNGETE QUELLO CHE VI SERVE */
int contatore = 0;
int permesso = -1;
int arrivo[N];

/* variabili per la sincronizzazione - AGGIUNGETE QUELLO CHE VI SERVE */
pthread_mutex_t mutex;
pthread_cond_t condAttesa;
pthread_cond_t condFine[N];
pthread_cond_t condUscita;

void *Uscita (void *arg) 
{ 
	char Plabel[128] = "";
	int i;

	DBGpthread_mutex_lock(&mutex, Plabel);

	DBGpthread_cond_wait(&condUscita, &mutex, Plabel);

	for (i=0; i<N; i++) {
		permesso = arrivo[i];
		printf("esce %d\n", permesso);
		DBGpthread_cond_broadcast(&condFine[permesso], Plabel);
		DBGpthread_cond_wait(&condUscita, &mutex, Plabel);
	}

	DBGpthread_mutex_unlock(&mutex, Plabel);

	pthread_exit(NULL); 
}

void *Thread (void *arg) 
{ 
	char Plabel[128];
	intptr_t indice;

	indice=(intptr_t)arg;
	sprintf(Plabel,"T%" PRIiPTR "",indice);

	DBGpthread_mutex_lock(&mutex, Plabel);
	printf("%s entra in coda (%d)\n", Plabel, contatore);
	arrivo[contatore] = indice;
	contatore++;
	DBGpthread_cond_broadcast(&condAttesa, Plabel); 
	while ( contatore != 5 ) {
		DBGpthread_cond_wait(&condAttesa, &mutex, Plabel);
	}

	DBGpthread_cond_signal(&condUscita, Plabel);
	
	DBGpthread_cond_wait(&condFine[indice], &mutex, Plabel);
	
	printf("%s esce dalla coda\n", Plabel);
	DBGpthread_cond_signal(&condUscita, Plabel);
	DBGpthread_mutex_unlock(&mutex, Plabel);

	pthread_exit(NULL); 
}

int main ( int argc, char* argv[] ) 
{ 
	pthread_t th[N];
	pthread_t u;
	int  rc;
	uintptr_t i=0;
	void *ptr;

	/* INIZIALIZZATE LE VOSTRE VARIABILI CONDIVISE e tutto quel che serve - fate voi */
	rc = pthread_cond_init(&condAttesa, NULL);
	if(rc) PrintERROR_andExit(rc,"pthread_cond_init failed");
	rc = pthread_cond_init(&condUscita, NULL);
	if(rc) PrintERROR_andExit(rc,"pthread_cond_init failed");
	for(i=0;i<N;i++) {
		rc = pthread_cond_init(&condFine[i], NULL);
		if(rc) PrintERROR_andExit(rc,"pthread_cond_init failed");
	}
	rc = pthread_mutex_init(&mutex, NULL);
	if(rc) PrintERROR_andExit(rc,"pthread_mutex_init failed");

	/* CREAZIONE PTHREAD dei tiratori */
	rc=pthread_create(&u,NULL,Uscita,NULL); 
	if(rc) PrintERROR_andExit(rc,"pthread_create failed");
	for(i=0;i<N;i++) {
		rc=pthread_create(&(th[i]),NULL,Thread,(void*)i); 
		if(rc) PrintERROR_andExit(rc,"pthread_create failed");
	}

	for(i=0;i<N;i++) {
		rc=pthread_join(th[i],&ptr);
		if(rc) PrintERROR_andExit(rc,"pthread_join failed");
	}
	rc=pthread_join(u,ptr);
	if(rc) PrintERROR_andExit(rc,"pthread_join failed");

	rc = pthread_cond_destroy(&condAttesa);
	if(rc) PrintERROR_andExit(rc,"pthread_cond_destroy failed");
	rc = pthread_cond_destroy(&condUscita);
	if(rc) PrintERROR_andExit(rc,"pthread_cond_destroy failed");
	for(i=0;i<N;i++) {
		rc = pthread_cond_destroy(&condFine[i]);
		if(rc) PrintERROR_andExit(rc,"pthread_cond_destroy failed");
	}
	rc = pthread_mutex_destroy(&mutex);
	if(rc) PrintERROR_andExit(rc,"pthread_mutex_destroy failed");

	pthread_exit(NULL);
	return(0); 
} 
  
