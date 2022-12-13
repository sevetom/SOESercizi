#include <unistd.h> 
#include <stdlib.h>
#include <stdio.h> 

#include <stdint.h>    /* definizioni intptr_t */
#include <inttypes.h>  /* definizioni PRIiPTR */

#include <pthread.h> 
#include "printerror.h"

/* variabili globali  */
#define IPPOPOTAMI 2
#define LEMURI 7

/* variabili per la sincronizzazione */
int ippoattr = -1;
int groppe[IPPOPOTAMI];
pthread_mutex_t mutex; 
pthread_cond_t cond;

void *Ippopotamo (void *arg)
{
	int rc;
	intptr_t id = (intptr_t)arg;

	while(1) {
        rc = pthread_mutex_lock(&mutex);
        if ( rc ) PrintERROR_andExit(rc,"pthread_mutex_lock failed");

		/* Azione  */
		if ( ippoattr == -1 ) {
			printf("Sono Ippopotamo %" PRIiPTR " ed entro in acqua\n", id);
			ippoattr = id;
		}
		else {
			rc = pthread_cond_wait(&cond, &mutex);
			if ( rc ) PrintERROR_andExit(rc,"pthread_cond_wait failed");
		}

		rc = pthread_mutex_unlock(&mutex);
		if ( rc ) PrintERROR_andExit(rc,"pthread_mutex_unlock failed");

		rc = pthread_cond_broadcast(&cond);
		if ( rc ) PrintERROR_andExit(rc,"pthread_cond_broadcast failed");
		printf("Sono Ippopotamo %" PRIiPTR " e faccio salire in groppa\n", id);
		groppe[id] = 0;
		sleep(2);

		if( ippoattr == id ) {
			printf("Sono Ippopotamo %" PRIiPTR " e attraverso il guado\n", id);
			sleep(3);
			printf("Sono Ippopotamo %" PRIiPTR " sono arrivato\n", id);

			rc = pthread_mutex_lock(&mutex);
			if ( rc ) PrintERROR_andExit(rc,"pthread_mutex_lock failed");
			ippoattr = -1;
			groppe[id] = -1;
			rc = pthread_mutex_unlock(&mutex);
			if ( rc ) PrintERROR_andExit(rc,"pthread_mutex_unlock failed");

			rc = pthread_cond_broadcast(&cond);
			if ( rc ) PrintERROR_andExit(rc,"pthread_cond_broadcast failed");

			sleep(3);
			printf("Sono Ippopotamo %" PRIiPTR " sono stato ricapultato via\n", id);
		}
	}
	
	pthread_exit(NULL);
}

void *Lemure (void *arg)
{
	int rc;
	int salito = -1;
	int i;
	intptr_t id = (intptr_t)arg;

	while(1) {
        rc = pthread_mutex_lock(&mutex);
        if ( rc ) PrintERROR_andExit(rc,"pthread_mutex_lock failed");

		/* Azione  */
		for ( i=0; i<IPPOPOTAMI; i++ ) {
			if ( groppe[i] >= 0 && groppe[i] < 4 && salito == -1 ) {
				printf("Sono Lemure %" PRIiPTR " e salgo in groppa %d\n", id, i);
				groppe[i]++;
				salito = i;
			}
		}

		rc = pthread_cond_wait(&cond, &mutex);
		if ( rc ) PrintERROR_andExit(rc,"pthread_cond_wait failed");

		if ( salito > 0 && groppe[salito] == -1 ) {
			printf("Sono Lemure %" PRIiPTR " e scendo dalla groppa\n", id);
			salito = -1;
		}
        
		rc = pthread_mutex_unlock(&mutex);
        if ( rc ) PrintERROR_andExit(rc,"pthread_mutex_unlock failed");
	}
	
	pthread_exit(NULL);
}

int main () 
{ 
	pthread_t ip[IPPOPOTAMI];
	pthread_t le[LEMURI];
	intptr_t i;
	int  rc;
	void *ptr;

	for ( i=0; i<IPPOPOTAMI; i++ ) {
		groppe[i] = -1;
	}

	/* Inizializzazione di variabili per la sincronizzazione  */
	rc = pthread_cond_init(&cond, NULL);
	if( rc ) PrintERROR_andExit(rc,"pthread_cond_init failed");
	rc = pthread_mutex_init(&mutex, NULL); 
	if( rc ) PrintERROR_andExit(rc,"pthread_mutex_init failed");

	/* Inizializzazione dei threads  */
	for( i=0; i<IPPOPOTAMI; i++ ) {
		rc = pthread_create(&(ip[i]), NULL, Ippopotamo, (void*)i); 
		if (rc) PrintERROR_andExit(rc,"pthread_create failed");
	}

	for( i=0; i<LEMURI; i++ ) {
		rc = pthread_create(&(le[i]), NULL, Lemure, (void*)i); 
		if (rc) PrintERROR_andExit(rc,"pthread_create failed");
	}

	for(i=0;i<IPPOPOTAMI;i++) {
		rc = pthread_join(ip[i], &ptr ); 
		if (rc) PrintERROR_andExit(rc,"pthread_join failed");
	}

	for(i=0;i<LEMURI;i++) {
		rc = pthread_join(le[i], &ptr ); 
		if (rc) PrintERROR_andExit(rc,"pthread_join failed");
	}

	/* Deallocazione di variabili per la sincronizzazione  */
	rc = pthread_mutex_destroy(&mutex); 
	if(rc) PrintERROR_andExit(rc,"pthread_mutex_destroy failed");
	rc = pthread_cond_destroy(&cond); 
	if(rc) PrintERROR_andExit(rc,"pthread_cond_destroy failed");

	pthread_exit(NULL);
	return(0); 
} 
  
  
