#include <unistd.h> 
#include <stdlib.h>
#include <stdio.h> 

#include <stdint.h>    /* definizioni intptr_t */
#include <inttypes.h>  /* definizioni PRIiPTR */

#include <pthread.h> 
#include "printerror.h"

/* variabili globali  */
#define LEGIONARI 50
#define RASOI 30
#define SCODELLE 25

/* variabili per la sincronizzazione */
int rasoi=RASOI;
int scodelle=SCODELLE;
pthread_mutex_t mutexRasoi;
pthread_mutex_t mutexScodelle;
pthread_cond_t condAttesaRasoio;
pthread_cond_t condAttesaScodella;

void *Legionario (void *arg)
{
	int rc;
	intptr_t id = (intptr_t)arg;

		rc = pthread_mutex_lock(&mutexRasoi);
        if ( rc ) PrintERROR_andExit(rc,"pthread_mutex_lock failed");

		if ( rasoi <= 0 ) {
			printf("Sono il legionario %" PRIiPTR " e attendo il rasoio\n",id);
			rc = pthread_cond_wait(&condAttesaRasoio, &mutexRasoi);
			if ( rc ) PrintERROR_andExit(rc,"pthread_cond_wait failed");
		}
		rasoi--;
		printf("Sono il legionario %" PRIiPTR " e prendo il rasoio\n",id);

		rc = pthread_mutex_unlock(&mutexRasoi);
        if ( rc ) PrintERROR_andExit(rc,"pthread_mutex_unlock failed");

        rc = pthread_mutex_lock(&mutexScodelle);
        if ( rc ) PrintERROR_andExit(rc,"pthread_mutex_lock failed");

        if ( scodelle <= 0 ) {
			printf("Sono il legionario %" PRIiPTR " e attendo la scodella\n",id);
			rc = pthread_cond_wait(&condAttesaScodella, &mutexScodelle);
			if ( rc ) PrintERROR_andExit(rc,"pthread_cond_wait failed");
		}
		scodelle--;
		printf("Sono il legionario %" PRIiPTR " e prendo la scodella\n",id);
        
		rc = pthread_mutex_unlock(&mutexScodelle);
        if ( rc ) PrintERROR_andExit(rc,"pthread_mutex_unlock failed");

        printf("Sono il legionario %" PRIiPTR " e mi faccio la barba\n",id);
        sleep(5);
        
		rc = pthread_mutex_lock(&mutexRasoi);
        if ( rc ) PrintERROR_andExit(rc,"pthread_mutex_lock failed");
        rasoi++;
        rc = pthread_mutex_unlock(&mutexRasoi);
        if ( rc ) PrintERROR_andExit(rc,"pthread_mutex_unlock failed");

		printf("Sono il legionario %" PRIiPTR " e ridò il rasoio\n",id);
        rc = pthread_cond_broadcast(&condAttesaRasoio);
		if ( rc ) PrintERROR_andExit(rc,"pthread_cond_broadcast failed");

        rc = pthread_mutex_lock(&mutexScodelle);
        if ( rc ) PrintERROR_andExit(rc,"pthread_mutex_lock failed");
        scodelle++;
        rc = pthread_mutex_unlock(&mutexScodelle);
        if ( rc ) PrintERROR_andExit(rc,"pthread_mutex_unlock failed");

		printf("Sono il legionario %" PRIiPTR " e ridò la scodella\n",id);
        rc = pthread_cond_broadcast(&condAttesaScodella);
		if ( rc ) PrintERROR_andExit(rc,"pthread_cond_broadcast failed");
		/* Possibili Azioni
		 rc = pthread_cond_broadcast(&cond);
		if ( rc ) PrintERROR_andExit(rc,"pthread_cond_broadcast failed");
        rc = pthread_cond_wait(&cond, &mutex);
        if ( rc ) PrintERROR_andExit(rc,"pthread_cond_wait failed");
        rc = pthread_cond_signal(&cond);
		if ( rc ) PrintERROR_andExit(rc,"pthread_cond_signal failed");
		*/
	
	pthread_exit(NULL);
}

int main () 
{ 
	pthread_t leg[LEGIONARI];
	intptr_t i;
	int  rc;
	void *ptr;
	
	/* Inizializzazione di variabili per la sincronizzazione  */
	rc = pthread_cond_init(&condAttesaRasoio, NULL);
	if( rc ) PrintERROR_andExit(rc,"pthread_cond_init failed");
	rc = pthread_cond_init(&condAttesaScodella, NULL);
	if( rc ) PrintERROR_andExit(rc,"pthread_cond_init failed");
	rc = pthread_mutex_init(&mutexRasoi, NULL); 
	if( rc ) PrintERROR_andExit(rc,"pthread_mutex_init failed");
	rc = pthread_mutex_init(&mutexScodelle, NULL); 
	if( rc ) PrintERROR_andExit(rc,"pthread_mutex_init failed");

	/* Inizializzazione dei threads  */
	for( i=0; i<LEGIONARI; i++ ) {
		rc = pthread_create(&(leg[i]), NULL, Legionario, (void*)i); 
		if (rc) PrintERROR_andExit(rc,"pthread_create failed");
	}

	for(i=0;i<LEGIONARI;i++) {
		rc = pthread_join(leg[i], &ptr); 
		if (rc) PrintERROR_andExit(rc,"pthread_join failed");
	}

	/* Deallocazione di variabili per la sincronizzazione  */
	rc = pthread_mutex_destroy(&mutexRasoi); 
	if(rc) PrintERROR_andExit(rc,"pthread_mutex_destroy failed");
	rc = pthread_mutex_destroy(&mutexScodelle); 
	if(rc) PrintERROR_andExit(rc,"pthread_mutex_destroy failed");
	rc = pthread_cond_destroy(&condAttesaRasoio); 
	if(rc) PrintERROR_andExit(rc,"pthread_cond_destroy failed");
	rc = pthread_cond_destroy(&condAttesaScodella); 
	if(rc) PrintERROR_andExit(rc,"pthread_cond_destroy failed");

	pthread_exit(NULL);
	return(0); 
} 
  
  
