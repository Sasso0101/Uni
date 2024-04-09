#!/bin/bash
# Write a shell script that accepts a number greater than 2 from command line and prints the Fibonacci sequence up to that term.

if (( $# < 1 )); then
    echo "Not enough parameters"
    exit
fi
if (( $1 <= 2)); then
    echo "Enter a number greater than 2!"
    exit
fi

n1=1
n2=1
i=2
echo -n "Fibonacci sequence up to $1 values: 1 1 "
while (( i != $1 )); do
    temp=$n2
    n2=$(( $n1 + $n2 ))
    n1=$temp
    i=$(( i + 1 ))
    echo -n "$n2 "
done