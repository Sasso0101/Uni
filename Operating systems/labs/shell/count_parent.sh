#!/bin/bash
# Write a shell script that checks if a folder contains more files than its parent (non-recursive).
# If yes, it outputs the number of directory in the current folder, 
# otherwise it outputs the number of directories in the parent folder
# Tip: remember that the parent folder can be accessed via the ".." naming in paths

if (( $# < 1 )); then
    echo "Not enough parameters"
else
    cd "$1"
    current=$(find . -type f -maxdepth 1 | wc -l)
    parent=$(find .. -type f -maxdepth 1 | wc -l)
    if (( current > parent )); then
        echo "there are more files in the current folder"
        echo $(find . -type d -maxdepth 1 | wc -l)
    else
        echo "there are more files in the parent folder"
        echo $(find .. -type d -maxdepth 1 | wc -l)
    fi
fi