#include <stdlib.h>
#include <pthread.h>
#include <stdint.h>
#include <unistd.h>
#include <inttypes.h>
#include "printerror.h"

#define AUTOSEC 10
#define DESTRA 0
#define SINISTRA 1
#define LIBERO -1
#define AUTOSTANOTTE 100

int transitanti = 0;
int lato;

pthread_mutex_t mutex;
pthread_cond_t cond;

void *Auto(void *arg)
{
	int rc;
	intptr_t id = (intptr_t)arg;
	intptr_t direzione = (intptr_t)arg % 2;

	rc = pthread_mutex_lock(&mutex);
    if ( rc ) PrintERROR_andExit(rc,"pthread_mutex_lock failed");
	if ( (lato != direzione && lato != LIBERO) || transitanti >= 5 ) {
		printf( "Sono l'auto %" PRIiPTR " e mi metto in coda a %s\n", id, direzione == DESTRA ? "destra" : "sinistra");
		rc = pthread_cond_wait(&cond, &mutex);
        if ( rc ) PrintERROR_andExit(rc,"pthread_cond_wait failed");
	}
	rc = pthread_mutex_unlock(&mutex);
    if ( rc ) PrintERROR_andExit(rc,"pthread_mutex_unlock failed");

	printf( "Sono l'auto %" PRIiPTR " e sto passando da %s\n", id,  direzione == DESTRA ? "destra" : "sinistra");
	transitanti++;
	lato = direzione;
	sleep(1);
	transitanti--;
	printf( "Sono l'auto %" PRIiPTR " e sono passata\n", id );

	rc = pthread_mutex_lock(&mutex);
    if ( rc ) PrintERROR_andExit(rc,"pthread_mutex_lock failed");
	if ( transitanti <= 0 ) {
		printf( "Sono l'auto %" PRIiPTR " ed ero l'ultima della coda\n", id );
		lato = LIBERO;
		rc = pthread_cond_broadcast(&cond);
        if ( rc ) PrintERROR_andExit(rc,"pthread_cond_broadcast failed");
	}
	rc = pthread_mutex_unlock(&mutex);
    if ( rc ) PrintERROR_andExit(rc,"pthread_mutex_unlock failed");
	
	pthread_exit(NULL);
}

int main()
{
	pthread_t a[AUTOSTANOTTE];
	int sec;
	int coda = 0;
	intptr_t dir = 0;
	int rc;
	void *ptr;
	int i;

	rc = pthread_cond_init(&cond, NULL);
	if ( rc ) PrintERROR_andExit(rc, "pthread_cond_init failed");
	rc = pthread_mutex_init(&mutex, NULL);
	if ( rc ) PrintERROR_andExit(rc, "pthread_mutex_init failed");

	while(1) {
		/*
		* a = (*pthread_t)realloc(a, AUTOSEC*SEC);
		* if ( a==NULL ) PrintERROR_andExit(a, "Realloc failed");
		*/

		printf("Lancio 10 auto\n");
		for ( sec=coda; coda<(AUTOSEC+sec); coda++ ) {
			rc = pthread_create(&(a[coda]), NULL, Auto, (void*)dir);
			if ( rc ) PrintERROR_andExit(rc, "pthread_create failed");
			dir++;
		}

		for ( i=coda-AUTOSEC; i<AUTOSEC; i++ ) {
			rc = pthread_join(a[i], &ptr );
            if ( rc ) PrintERROR_andExit(rc,"pthread_join failed");
        }
		
		sleep(1);
	}

	rc = pthread_cond_destroy(&cond);
	if ( rc ) PrintERROR_andExit(rc, "pthread_cond_destroy failed");
	rc = pthread_mutex_destroy(&mutex);
	if ( rc ) PrintERROR_andExit(rc, "pthread_mutex_destroy failed");

	pthread_exit(NULL);
	return 0;
}
