/* file:  usoContemporaneo.c
   ogni thread prende uno strumento se c'e'
   e lo usa in contemporanea con altri thread,
   poi risveglia un thread in attesa.
*/ 


#include <unistd.h> 
#include <stdlib.h> 	/* per random e srandom */
#include <stdio.h> 

#include <stdint.h>    /* definizioni intptr_t */
#include <inttypes.h>  /* definizioni PRIiPTR */

#include <pthread.h> 
#include "printerror.h"
#include "DBGpthread.h"

#define NUMTHREADS 6
#define NUMSTRUMENTI 3


/* variabili per la sincronizzazione */
int contatore=NUMSTRUMENTI;
pthread_mutex_t  mutex; 
pthread_cond_t   cond;

void *Thread (void *arg) 
{ 
	uintptr_t mioindice;
	long int delay;
	int rc;
	char Label[512];

	mioindice=(intptr_t)arg;
	snprintf( Label, 512, "T%" PRIiPTR "", mioindice );

	while(1) {
                /* AGGIUNGI CODICE PER ATTENDERE CHE CI SIA
		   UNO STRUMENTO LIBERO E PER PRENDERLO
                */
        rc = pthread_mutex_lock(&mutex);
        if ( rc ) PrintERROR_andExit(rc,"pthread_mutex_lock failed");
		if ( contatore > 0) contatore--;
		else {
			rc = pthread_cond_wait(&cond, &mutex);
			if ( rc ) PrintERROR_andExit(rc,"pthread_cond_wait failed");
		}
		rc = pthread_mutex_unlock(&mutex);
        if ( rc ) PrintERROR_andExit(rc,"pthread_mutex_unlock failed");
                /* FINE AGGIUNTA */

		/* SEZIONE NON CRITICA: uso lo strumento per
		   un numero variabile di secondi tra 1 e 3
		*/
		printf("Sono %s ed uso lo strumento\n", Label);
		fflush(stdout);
		delay=1+(random()%3);
		/* uso lo strumento per qualche secondo */
		sleep(delay);

                /* AGGIUNGI CODICE PER RILASCIARE LO STRUMENTO 
		   E AVVISARE GLI ATRI THREAD CHE ORA
		   C'E' UNO STRUMENTO LIBERO
                */
        rc = pthread_mutex_lock(&mutex);
        if ( rc ) PrintERROR_andExit(rc,"pthread_mutex_lock failed");
        contatore++;
        rc = pthread_cond_signal(&cond);
        if ( rc ) PrintERROR_andExit(rc,"pthread_cond_signal failed");
		rc = pthread_mutex_unlock(&mutex);
        if ( rc ) PrintERROR_andExit(rc,"pthread_mutex_unlock failed");
                /* FINE AGGIUNTA */

		/* FACCIO QUALCOSA FUORI DA SEZIONE CRITICA ... */
		sleep(2);
	}
	pthread_exit(NULL); 
} 

int main () 
{ 
	pthread_t    th[NUMTHREADS]; 
	int  rc;
	intptr_t i;
	void *ptr; 

	rc = pthread_cond_init(&cond, NULL);
	if( rc ) PrintERROR_andExit(rc,"pthread_cond_init failed");
	rc = pthread_mutex_init(&mutex, NULL); 
	if( rc ) PrintERROR_andExit(rc,"pthread_mutex_init failed");

	/* inizializzo generatore numeri casuali */
	srandom( time(NULL) );

	for( i=0; i<NUMTHREADS; i++ ) {
		rc = pthread_create(&(th[i]), NULL, Thread, (void*)i); 
		if (rc) PrintERROR_andExit(rc,"pthread_create failed");
	}

	for(i=0;i<NUMTHREADS;i++) {
		rc = pthread_join(th[i], &ptr ); 
		if (rc) PrintERROR_andExit(rc,"pthread_join failed");
	}

	rc = pthread_mutex_destroy(&mutex); 
	if(rc) PrintERROR_andExit(rc,"pthread_mutex_destroy failed");
	rc = pthread_cond_destroy(&cond); 
	if(rc) PrintERROR_andExit(rc,"pthread_cond_destroy failed");

	return(0); 
} 
  
  
