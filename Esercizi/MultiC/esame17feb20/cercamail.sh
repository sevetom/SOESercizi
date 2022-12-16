#!/bin/bash

NOME=$1
COGNOME=$2
MATRICOLA=$3

while read MAT EMAIL ; do
    if [[ ${MATRICOLA} == ${MAT} ]] ; then
        echo "la email dello studente ${NOME} ${COGNOME} ${MATRICOLA} e' ${EMAIL}"
    fi
done < email.txt
