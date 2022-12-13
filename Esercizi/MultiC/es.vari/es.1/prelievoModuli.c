#include <unistd.h> 
#include <stdlib.h>
#include <stdio.h> 

#include <stdint.h>    /* definizioni intptr_t */
#include <inttypes.h>  /* definizioni PRIiPTR */

#include <pthread.h> 
#include "printerror.h"

#define STUDENTI 50
#define MODULI 3


/* variabili per la sincronizzazione */
int contatore=MODULI;
int aspettanti=STUDENTI;
pthread_mutex_t mutex; 
pthread_cond_t cond;

void *Impiegata (void *arg)
{
	int rc;

	while(aspettanti > 0) {
        rc = pthread_mutex_lock(&mutex);
        if ( rc ) PrintERROR_andExit(rc,"pthread_mutex_lock failed");

		printf("Sono l'impiegata e mi tocca lavorare\n");
		sleep(2);
		contatore = MODULI;

		rc = pthread_cond_broadcast(&cond);
		if ( rc ) PrintERROR_andExit(rc,"pthread_cond_broadcast failed");
		
        rc = pthread_cond_wait(&cond, &mutex);
        if ( rc ) PrintERROR_andExit(rc,"pthread_cond_wait failed");
        
		rc = pthread_mutex_unlock(&mutex);
        if ( rc ) PrintERROR_andExit(rc,"pthread_mutex_unlock failed");
	}
	printf("La segretaria se ne va a casina\n");
	pthread_exit(NULL);
}

void *Studente (void *arg) 
{ 
	uintptr_t mioindice;
	int rc;
	char preso = 0;
	char Label[512];

	mioindice=(intptr_t)arg;
	snprintf( Label, 512, "S%" PRIiPTR "", mioindice );

	while(!preso) {
        rc = pthread_mutex_lock(&mutex);
        if ( rc ) PrintERROR_andExit(rc,"pthread_mutex_lock failed");
		if ( contatore <= 0) {
			rc = pthread_cond_wait(&cond, &mutex);
			if ( rc ) PrintERROR_andExit(rc,"pthread_cond_wait failed");
		}
		else {
			printf("Sono %s e prendo il modulo\n", Label);
			sleep(1);
			contatore--;
			printf("Sono rimasti %d moduli\n",contatore);
			preso = 1;
			aspettanti--;
			if ( contatore <= 0 || aspettanti <= 0) {
				printf("Sono %s e chiamo l'impiegata\n", Label);
				rc = pthread_cond_signal(&cond);
				if ( rc ) PrintERROR_andExit(rc,"pthread_cond_signal failed");
			}
		}
		rc = pthread_mutex_unlock(&mutex);
        if ( rc ) PrintERROR_andExit(rc,"pthread_mutex_unlock failed");
	}
	pthread_exit(NULL);
} 

int main () 
{ 
	pthread_t st[STUDENTI];
	pthread_t seg;
	int  rc;
	intptr_t i;
	void *ptr; 

	rc = pthread_cond_init(&cond, NULL);
	if( rc ) PrintERROR_andExit(rc,"pthread_cond_init failed");
	rc = pthread_mutex_init(&mutex, NULL); 
	if( rc ) PrintERROR_andExit(rc,"pthread_mutex_init failed");

	for( i=0; i<STUDENTI; i++ ) {
		rc = pthread_create(&(st[i]), NULL, Studente, (void*)i); 
		if (rc) PrintERROR_andExit(rc,"pthread_create failed");
	}
	rc = pthread_create(&seg, NULL, Impiegata, NULL); 
	if (rc) PrintERROR_andExit(rc,"pthread_create failed");

	rc = pthread_join(seg, &ptr ); 
	if (rc) PrintERROR_andExit(rc,"pthread_join failed");
	for(i=0;i<STUDENTI;i++) {
		rc = pthread_join(st[i], &ptr ); 
		if (rc) PrintERROR_andExit(rc,"pthread_join failed");
	}

	rc = pthread_mutex_destroy(&mutex); 
	if(rc) PrintERROR_andExit(rc,"pthread_mutex_destroy failed");
	rc = pthread_cond_destroy(&cond); 
	if(rc) PrintERROR_andExit(rc,"pthread_cond_destroy failed");

	pthread_exit(NULL);
	return(0); 
} 
  
  
