#include <unistd.h> 
#include <stdlib.h>
#include <stdio.h> 

#include <stdint.h>    /* definizioni intptr_t */
#include <inttypes.h>  /* definizioni PRIiPTR */

#include <pthread.h> 
#include "printerror.h"

/* variabili globali  */
#define GLOBAL 10

/* variabili per la sincronizzazione */
int sinc=GLOBAL;
pthread_mutex_t mutex; 
pthread_cond_t cond;

void *Thread (void *arg)
{
	int rc;

	while(1) {
        rc = pthread_mutex_lock(&mutex);
        if ( rc ) PrintERROR_andExit(rc,"pthread_mutex_lock failed");

		/* Azione  */

		/* Possibili Azioni
		 rc = pthread_cond_broadcast(&cond);
		if ( rc ) PrintERROR_andExit(rc,"pthread_cond_broadcast failed");
        rc = pthread_cond_wait(&cond, &mutex);
        if ( rc ) PrintERROR_andExit(rc,"pthread_cond_wait failed");
        rc = pthread_cond_signal(&cond);
		if ( rc ) PrintERROR_andExit(rc,"pthread_cond_signal failed");
		*/
        
		rc = pthread_mutex_unlock(&mutex);
        if ( rc ) PrintERROR_andExit(rc,"pthread_mutex_unlock failed");
	}
	
	pthread_exit(NULL);
}

int main () 
{ 
	pthread_t ids[GLOBAL];
	intptr_t i;
	int  rc;
	void *ptr;
	
	/* Inizializzazione di variabili per la sincronizzazione  */
	rc = pthread_cond_init(&cond, NULL);
	if( rc ) PrintERROR_andExit(rc,"pthread_cond_init failed");
	rc = pthread_mutex_init(&mutex, NULL); 
	if( rc ) PrintERROR_andExit(rc,"pthread_mutex_init failed");

	/* Inizializzazione dei threads  */
	for( i=0; i<GLOBAL; i++ ) {
		rc = pthread_create(&(ids[i]), NULL, Studente, (void*)i); 
		if (rc) PrintERROR_andExit(rc,"pthread_create failed");
	}

	for(i=0;i<GLOBAL;i++) {
		rc = pthread_join(ids[i], &ptr ); 
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
  
  
