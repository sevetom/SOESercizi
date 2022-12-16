#!/bin/bash

while read MATRICOLA EMAIL ; do
    ./cercastudente.sh ${EMAIL} ${MATRICOLA}
done < email.txt