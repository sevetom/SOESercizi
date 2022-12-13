#!/bin/bash

while read MAT EMAIL ; do
	./cercastudente.sh $EMAIL $MAT
done < email.txt
