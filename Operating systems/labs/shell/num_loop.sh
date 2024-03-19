#!/bin/bash

for i in {1..20}
do
	echo $i
	if [ $i -gt 10 ]; then
		echo "I'm big!"
	fi
done
