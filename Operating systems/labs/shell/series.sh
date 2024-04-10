#!/bin/bash
# Write a shell script that accepts a number n greater than 0 from command line. 
# and prints all the value n of the following series Si
# S0 = 0, 
# Si = 2*i + i^2 + Si-1, if i is even (2, 4, 6, 8 ...)
# Si = -2*i + i^2 + Si-1, if i is odd (1, 3, 5, 7 ...)

if (( $# < 1 )); then
    echo "Not enough parameters!"
    exit
fi

total=0

for (( i=1; i <= $1; i++ )); do
    if (( $i % 2 == 0 )); then
        total=$(( $total + 2*$i + $i*$i ))
    else
        total=$(( $total - 2*$i + $i*$i ))
    fi
done

echo "Total: $total"