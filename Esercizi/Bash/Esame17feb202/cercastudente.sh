#!/bin/bash

EMAIL=$1
MATRICOLA=$2

while read NOME COGNOME MAT IND ; do
	if [[ $MATRICOLA == $MAT ]] ; then
		echo "la account $EMAIL appartiene allo studente $NOME $COGNOME."
	fi
done < matricola.txt
