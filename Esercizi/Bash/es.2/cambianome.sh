#!/bin/bash

j=10
for (( i=0; $i<10; i=$i+1 )) ; do
	(( j=$j - 1 ))
	mv 1.$i 2.$j
done
