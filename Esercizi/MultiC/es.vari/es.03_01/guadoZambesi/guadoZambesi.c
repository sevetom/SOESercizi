/* file:  guadoZambesi.c */


#include <unistd.h> 
#include <stdlib.h> 
#include <stdio.h> 
#include <stdint.h>
#include <inttypes.h>
#include <pthread.h> 

#include "printerror.h"
#include "DBGpthread.h"

#define NUMIPPOPOTAMI 2
#define NUMLEMURI 7

/* dati da proteggere */
int NLemuriSuIppopotamo=0;
int LemuriPossonoSalireSuIppopotamo=0;

/* variabili per la sincronizzazione */
pthread_mutex_t  mutexGuado, /* solo un ippopotamo entra in acqua */
		 mutexLemuri; 
pthread_cond_t   condIppoEntraInAcquaRivaNord,
		 condIppoEsceRivaSudProd;

void *Ippopotamo (void *arg) 
{ 
	char Plabel[128];
	intptr_t indice;

	indice=(intptr_t)arg;
	sprintf(Plabel,"Ippo%" PRIiPTR "",indice);

	while(1) {
		/* solo un ippopotamo alla volta entra in acqua */
		DBGpthread_mutex_lock(&mutexGuado,Plabel); 

		/* proteggo le variabili globali */
		/* consento ai lemuri di salire sull'ippopotamo */
		DBGpthread_mutex_lock(&mutexLemuri,Plabel); 
		printf("ippo %s entra in acqua\n", Plabel);
		fflush(stdout);
		NLemuriSuIppopotamo=0;
		LemuriPossonoSalireSuIppopotamo=1;
		DBGpthread_cond_broadcast(&condIppoEntraInAcquaRivaNord,Plabel); 
		DBGpthread_mutex_unlock(&mutexLemuri,Plabel); 
		/* ora i lemuri possono salire */

		sleep(2); /* ci impiega 2 sec a entrare in acqua */

		/* ora i lemuri non possono piu' salire */
		DBGpthread_mutex_lock(&mutexLemuri,Plabel); 
		printf("lemuri non possono piu' salire\n");
		printf("ippo %s nuota verso riva sud\n", Plabel);
		fflush(stdout);
		LemuriPossonoSalireSuIppopotamo=0;
		DBGpthread_mutex_unlock(&mutexLemuri,Plabel); 

		sleep(3); /* ci impiega 3 sec a attraversare */

		/* scaravento i lemuri per terra */
		printf("ippo %s raggiunge riva sud, giu' i lemuri\n", Plabel);
		fflush(stdout);
		DBGpthread_cond_broadcast(&condIppoEsceRivaSudProd,Plabel); 

		/* rilascio mutua esclusione sul guado */
		DBGpthread_mutex_unlock(&mutexGuado,Plabel); 

		/* tromba d'aria, si torna alla riva Nord in 3 sec */
		printf("ippo sollevato da tromba d'aria\n");
		fflush(stdout);
		sleep(3);
		printf("ippo di nuovo in riva nord\n");
		fflush(stdout);
	}
	pthread_exit(NULL); 
} 

void *Lemure (void *arg) 
{ 
	char Plabel[128];
	intptr_t indice;

	indice=(intptr_t)arg;
	sprintf(Plabel,"Lem%" PRIiPTR "",indice);

	while(1) {
		/* COMPLETARE DA QUI */
		/* il lemure aspetta di poter salire sull'ippo */
		DBGpthread_mutex_lock(&mutexLemuri,Plabel); 
		while (
			LemuriPossonoSalireSuIppopotamo==0
			||
			NLemuriSuIppopotamo>=4
		      )
			DBGpthread_cond_wait(&condIppoEntraInAcquaRivaNord,&mutexLemuri,Plabel);

		/* lemure sale su ippopotamo */
		NLemuriSuIppopotamo++;

		printf("lemure sale su ippo\n");
		fflush(stdout);

		/* aspetto che ippopotamo raggiunga riva nord */
		DBGpthread_cond_wait(&condIppoEsceRivaSudProd,&mutexLemuri,Plabel);

		/* sono in riva sud */
		printf("lemure arriva riva sud\n");
		fflush(stdout);
		DBGpthread_mutex_unlock(&mutexLemuri,Plabel); 


		/* COMPLETARE FINO A QUI */
		/* tromba d'aria, si torna in riva Nord in 3 sec */
		printf("lemure trascinato da tromba d'aria\n");
		fflush(stdout);
		sleep(3);
		printf("lemure di nuovo in riva nord\n");
		fflush(stdout);
	}
	pthread_exit(NULL); 
}


int main ( int argc, char* argv[] ) 
{ 
	pthread_t    th; 
	int  rc;
	intptr_t i;

	rc = pthread_cond_init(&condIppoEntraInAcquaRivaNord, NULL);
	if( rc ) PrintERROR_andExit(rc,"pthread_cond_init failed");
	rc = pthread_cond_init(&condIppoEsceRivaSudProd, NULL);
	if( rc ) PrintERROR_andExit(rc,"pthread_cond_init failed");
	rc = pthread_mutex_init(&mutexGuado, NULL); 
	if( rc ) PrintERROR_andExit(rc,"pthread_mutex_init failed");
	rc = pthread_mutex_init(&mutexLemuri, NULL); 
	if( rc ) PrintERROR_andExit(rc,"pthread_mutex_init failed");


	for(i=0;i<NUMIPPOPOTAMI;i++) {
		rc=pthread_create(&th,NULL,Ippopotamo,(void*)i); 
		if(rc) PrintERROR_andExit(rc,"pthread_create failed");
	}
	for(i=0;i<NUMLEMURI;i++) {
		rc=pthread_create(&th,NULL,Lemure,(void*)i); 
		if(rc) PrintERROR_andExit(rc,"pthread_create failed");
	}
	
	pthread_exit(NULL);
	return(0); 
} 
  
  
  
