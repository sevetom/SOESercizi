#include <unistd.h> 
#include <stdlib.h>
#include <stdio.h> 

#include <stdint.h>    /* definizioni intptr_t */
#include <inttypes.h>  /* definizioni PRIiPTR */

#include <pthread.h>
#include <stdbool.h>
#include "printerror.h"
#include "DBGpthread.h"

/* variabili globali  */
#define CONIGLI 5

/* variabili per la sincronizzazione */
int tana = 0;
bool libera = true;
pthread_mutex_t mutex; 
pthread_cond_t condTana;
pthread_cond_t condPartner;

void *Coniglio(void *arg)
{
	int rc;
	pthread_t figlio;
	char Label[512];
	intptr_t id = (intptr_t)arg + 1;
	sprintf(Label, "C%" PRIiPTR "", id);

	DBGpthread_mutex_lock(&mutex, Label);

	while ( tana == 2 || !libera ) {
		printf("%s aspetto\n", Label);
		DBGpthread_cond_wait(&condTana, &mutex, Label);
	}

	tana++;
	printf("%s entra nella tana (%d)\n", Label, tana);
	while ( tana < 2 ) DBGpthread_cond_wait(&condPartner, &mutex, Label);
	libera = false;

	DBGpthread_mutex_unlock(&mutex, Label);

	DBGpthread_cond_signal(&condPartner, Label);

	printf("%s mi riproduco\n", Label);
	DBGsleep(2, Label);
	rc = pthread_create(&figlio, NULL, Coniglio, (void*)(id+6)); 
	if (rc) PrintERROR_andExit(rc,"pthread_create failed");

	DBGpthread_mutex_lock(&mutex, Label);
	tana--;
	printf("%s esco dalla tana\n", Label);
	if ( tana == 0 ) {
		printf("Avanti Favoia\n");
		libera = true;
		DBGpthread_cond_broadcast(&condTana, Label);
	}
	DBGpthread_mutex_unlock(&mutex, Label);
	
	pthread_exit(NULL);
}

int main () 
{ 
	pthread_t con[CONIGLI];
	intptr_t i;
	int  rc;
	void *ptr;
	
	/* Inizializzazione di variabili per la sincronizzazione  */
	rc = pthread_cond_init(&condTana, NULL);
	if( rc ) PrintERROR_andExit(rc,"pthread_cond_init failed");
	rc = pthread_cond_init(&condPartner, NULL);
	if( rc ) PrintERROR_andExit(rc,"pthread_cond_init failed");
	rc = pthread_mutex_init(&mutex, NULL); 
	if( rc ) PrintERROR_andExit(rc,"pthread_mutex_init failed");

	/* Inizializzazione dei threads  */
	for( i=0; i<CONIGLI; i++ ) {
		rc = pthread_create(&(con[i]), NULL, Coniglio, (void*)i); 
		if (rc) PrintERROR_andExit(rc,"pthread_create failed");
	}

	for(i=0;i<CONIGLI;i++) {
		rc = pthread_join(con[i], &ptr ); 
		if (rc) PrintERROR_andExit(rc,"pthread_join failed");
	}

	/* Deallocazione di variabili per la sincronizzazione  */
	rc = pthread_mutex_destroy(&mutex); 
	if(rc) PrintERROR_andExit(rc,"pthread_mutex_destroy failed");
	rc = pthread_cond_destroy(&condTana); 
	if(rc) PrintERROR_andExit(rc,"pthread_cond_destroy failed");
	rc = pthread_cond_destroy(&condPartner); 
	if(rc) PrintERROR_andExit(rc,"pthread_cond_destroy failed");

	pthread_exit(NULL);
	return(0); 
} 
  
  
