#!/bin/bash
# Write a shell script that accepts a number input by user and check if it is prime or not. 
# Using function is must here

is_prime() {
    i=2
    while (( $i <= $1/2 )) && (( $1 % $i != 0 )); do
        i=$(( i + 1 ))
    done

    if (( $i <= $1/2 )); then
        echo "Number is not prime! First divisor is: $i"
    else
        echo "Number is prime!"
    fi
}

is_prime $1