#!/bin/bash

while read NOME COGNOME MAT IND ; do
	./cercaemail.sh $NOME $COGNOME $MAT
done < matricola.txt
