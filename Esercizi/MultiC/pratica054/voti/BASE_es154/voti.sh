#!/bin/bash
### Nei commenti che seguono vedete dei suggerimenti sull'algoritmo da
### seguire per produrre l'output corretto.
###

# I voti della seconda prova li devo mettere tutti nello standard output
cat esame2.txt
# Poi devo mettere nello standard output i voti della prima prova
# ottenuti dagli studenti che non hanno un voto nella seconda prova
while read MAT1 VOTO1 ; do
    OUT = $( grep ${MAT1} esame2.txt )
    if [[ ${OUT} == "" ]] ; then
        echo "${MAT1} ${OUT}"
    fi
done < esame1.txt
