#!/bin/bash

if (( $# != 1 )) ; then 
	echo "numero argomenti errato" 1>&2 
       	exit 1
fi

if ! [[ -f $1 ]] ; then
	echo "argomento non file" 1>&2
	exit 2
fi

for (( i=0; $i<5; i=$i+1 )) ; do
	./goblin $1 &
done
exit 0
