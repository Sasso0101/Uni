#!/bin/bash
# Write a shell script that accepts a string as input, checks if it ends with .sh, if yes prints to screen the contents of the script and executes it. If not, prints "not a script" 
# Tip: use the 'test -f <filename> condition for checking if a file exists

read filename

if [[ $filename =~ .*\.sh$ ]]; then
    if [[ -f $filename ]]; then
        cat "$filename"
    else
        echo "File does not exist"
    fi
else
    echo "Not a script"
fi