#!/bin/bash
# Write a shell script that accepts a parameter n, checks if it is a number and if yes prints n lines with the pattern
# x
# xx
# xxx
# xxxx
# .......
# xxxxxxxxxxx (n times)
# Tip: remember that you can concatenate strings as var="${var}x"

if (( $# < 1 )); then
    echo "Not enough parameters!"
    exit
fi

if ! [[ $1 =~ ^[0-9]+$ ]]; then
    echo "Not a number!"
    exit
fi

for ((i=1; i<=$1; i++)); do
    string=""
    for ((j=1; j<=$i; j++)); do
        string="${string}x"
    done
    echo $string
done