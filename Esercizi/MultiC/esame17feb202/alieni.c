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
#define ALIENI 5
#define POSTI 2

/* variabili per la sincronizzazione */
int casa = 0;
bool occupata = false;
int attesa = 0;
pthread_mutex_t mutex;
pthread_cond_t condEntrata;
pthread_cond_t condUscita;

void *Alieno(void *arg)
{
	pthread_t na[2];
	int i;
	int rc;
	bool atteso = false;
	char Label[512];
	intptr_t id = (intptr_t)arg + 1;
	sprintf(Label, "A%" PRIiPTR "", id);

	printf("%s e' nato\n", Label);

	DBGpthread_mutex_lock(&mutex, Label);

	while ( occupata ) {
		if ( !atteso ) {
			attesa++;
			atteso = true;
			printf("%s si mette in coda: %d (%d)\n", Label, attesa, casa);
		}
		DBGpthread_cond_wait(&condUscita, &mutex, Label);
	}

	if ( atteso ) attesa--;
	casa++;
	printf("%s entra in casa (%d) coda: %d\n", Label, casa, attesa);
	DBGpthread_cond_signal(&condEntrata, Label);
	if ( casa == POSTI ) occupata = true;

	DBGpthread_mutex_unlock(&mutex, Label);

	printf("%s sta morendo e sta cercando di uscire di casa\n", Label);
	DBGsleep(2, Label);

	DBGpthread_mutex_lock(&mutex, Label);

	if ( casa == 1 ) DBGpthread_cond_wait(&condEntrata, &mutex, Label);
	casa--;
	printf("%s e' riuscito ad uscire di casa (%d)\n", Label, casa);
	occupata = false;
	DBGpthread_cond_broadcast(&condUscita, Label);
	if ( attesa >= 3 ) {
		printf("%s crea nuovi alieni\n", Label);
		for( i=0; i<2; i++ ) {
			rc = pthread_create(&(na[i]), NULL, Alieno, (void*)(id + 5)); 
			if (rc) PrintERROR_andExit(rc,"pthread_create failed");
		}
	}

	DBGpthread_mutex_unlock(&mutex, Label);
	
	printf("%s e' morto\n", Label);
	pthread_exit(NULL);
}

int main () 
{ 
	pthread_t al[ALIENI];
	intptr_t i;
	int  rc;
	void *ptr;
	
	/* Inizializzazione di variabili per la sincronizzazione  */
	rc = pthread_cond_init(&condEntrata, NULL);
	if( rc ) PrintERROR_andExit(rc,"pthread_cond_init failed");
	rc = pthread_cond_init(&condUscita, NULL);
	if( rc ) PrintERROR_andExit(rc,"pthread_cond_init failed");
	rc = pthread_mutex_init(&mutex, NULL); 
	if( rc ) PrintERROR_andExit(rc,"pthread_mutex_init failed");

	/* Inizializzazione dei threads  */
	for( i=0; i<ALIENI; i++ ) {
		rc = pthread_create(&(al[i]), NULL, Alieno, (void*)i); 
		if (rc) PrintERROR_andExit(rc,"pthread_create failed");
	}

	for(i=0;i<ALIENI;i++) {
		rc = pthread_join(al[i], &ptr ); 
		if (rc) PrintERROR_andExit(rc,"pthread_join failed");
	}

	/* Deallocazione di variabili per la sincronizzazione  */
	rc = pthread_mutex_destroy(&mutex); 
	if(rc) PrintERROR_andExit(rc,"pthread_mutex_destroy failed");
	rc = pthread_cond_destroy(&condEntrata); 
	if(rc) PrintERROR_andExit(rc,"pthread_cond_destroy failed");
	rc = pthread_cond_destroy(&condUscita); 
	if(rc) PrintERROR_andExit(rc,"pthread_cond_destroy failed");

	pthread_exit(NULL);
	return(0); 
} 
  
  
