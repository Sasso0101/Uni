#!/bin/bash
# Print number of lines of all files in current folder
for file in $(find . -type f -maxdepth 1)
do
	echo $(wc -l $file)
done
