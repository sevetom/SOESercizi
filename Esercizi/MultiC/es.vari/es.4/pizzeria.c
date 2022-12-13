#include <unistd.h> 
#include <stdlib.h>
#include <stdio.h> 

#include <stdint.h>    /* definizioni intptr_t */
#include <inttypes.h>  /* definizioni PRIiPTR */

#include <pthread.h> 
#include "printerror.h"

/* variabili globali  */
#define BANCONE 5

/* variabili per la sincronizzazione */
int pizze=BANCONE;
pthread_mutex_t mutex; 
pthread_cond_t condPizzaiolo;
pthread_cond_t condClienti;

void *Pizzaiolo (void *arg)
{
	int rc;

	while(1) {
        rc = pthread_mutex_lock(&mutex);
        if ( rc ) PrintERROR_andExit(rc,"pthread_mutex_lock failed");

		if ( pizze >= BANCONE ) {
			rc = pthread_cond_wait(&condPizzaiolo, &mutex);
			if ( rc ) PrintERROR_andExit(rc,"pthread_cond_wait failed");
		}
        
		rc = pthread_mutex_unlock(&mutex);
        if ( rc ) PrintERROR_andExit(rc,"pthread_mutex_unlock failed");

        printf("Il pizzaiolo prepara una pizza\n");
		sleep(1);
		pizze++;
		rc = pthread_cond_broadcast(&condClienti);
		if ( rc ) PrintERROR_andExit(rc,"pthread_cond_broadcast failed");
	}
	
	pthread_exit(NULL);
}

void *Mangiatore (void *arg) {
	intptr_t n = (intptr_t)arg;
	sleep(2);
	pizze--;
	printf("cliente %" PRIiPTR " ha mangiato una pizza\n", n);
	pthread_exit(NULL);
}

void *Clienti (void *arg) {
	int rc;
	pthread_t mn;
	intptr_t n = 0;
	void *ptr;

	while(1) {
		n++;
        rc = pthread_mutex_lock(&mutex);
        if ( rc ) PrintERROR_andExit(rc,"pthread_mutex_lock failed");

		if ( pizze < 1 ) {
			printf("Son finite le pizze!\n");
			rc = pthread_cond_signal(&condPizzaiolo);
			if ( rc ) PrintERROR_andExit(rc,"pthread_cond_signal failed");
			rc = pthread_cond_wait(&condClienti, &mutex);
			if ( rc ) PrintERROR_andExit(rc,"pthread_cond_wait failed");
		}

		rc = pthread_mutex_unlock(&mutex);
        if ( rc ) PrintERROR_andExit(rc,"pthread_mutex_unlock failed");
		
		printf("cliente %" PRIiPTR " prende una pizza\n", n);
		rc = pthread_create(&mn, NULL, Mangiatore,(void*)n); 
		if (rc) PrintERROR_andExit(rc,"pthread_create failed");
		rc = pthread_join(mn, &ptr); 
		if (rc) PrintERROR_andExit(rc,"pthread_join failed");
	}
	
	pthread_exit(NULL);
}

int main () 
{ 
	pthread_t pz;
	pthread_t cl;
	int  rc;
	void *ptr;
	
	/* Inizializzazione di variabili per la sincronizzazione  */
	rc = pthread_cond_init(&condPizzaiolo, NULL);
	if( rc ) PrintERROR_andExit(rc,"pthread_cond_init failed");
	rc = pthread_cond_init(&condClienti, NULL);
	if( rc ) PrintERROR_andExit(rc,"pthread_cond_init failed");
	rc = pthread_mutex_init(&mutex, NULL); 
	if( rc ) PrintERROR_andExit(rc,"pthread_mutex_init failed");

	/* Inizializzazione dei threads  */
	rc = pthread_create(&pz, NULL, Pizzaiolo, NULL); 
	if (rc) PrintERROR_andExit(rc,"pthread_create failed");
	rc = pthread_create(&cl, NULL, Clienti, NULL); 
	if (rc) PrintERROR_andExit(rc,"pthread_create failed");

	rc = pthread_join(pz, &ptr ); 
	if (rc) PrintERROR_andExit(rc,"pthread_join failed");
	rc = pthread_join(cl, &ptr ); 
	if (rc) PrintERROR_andExit(rc,"pthread_join failed");

	/* Deallocazione di variabili per la sincronizzazione  */
	rc = pthread_mutex_destroy(&mutex); 
	if(rc) PrintERROR_andExit(rc,"pthread_mutex_destroy failed");
	rc = pthread_cond_destroy(&condPizzaiolo); 
	if(rc) PrintERROR_andExit(rc,"pthread_cond_destroy failed");
	rc = pthread_cond_destroy(&condClienti); 
	if(rc) PrintERROR_andExit(rc,"pthread_cond_destroy failed");

	pthread_exit(NULL);
	return(0); 
} 
  
  
