#!/bin/bash

while read NOME COGNOME MATRICOLA IND ; do
    ./cercamail.sh ${NOME} ${COGNOME} ${MATRICOLA}
done < matricola.txt