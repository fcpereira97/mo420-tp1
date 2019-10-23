#!/bin/bash

counter=1
for filename in input/*; do
	echo $counter " $filename"
	./relaxlag-SG-P "$filename" "output/o_""${filename:6}" 10
	counter=$((counter+1))
done