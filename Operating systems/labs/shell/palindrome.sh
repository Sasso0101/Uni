#!/bin/bash 
# Write a script that accepts a number as parameter and checks if it is a palindrome

if [[ $# < 1 ]]; then
    echo "Not enough parameters!"
    exit
fi

if ! [[ $1 =~ ^[0-9]+$ ]]; then
    echo "Not a number!"
    exit
fi

temp=$1
rev=0

while (( $temp != 0 )); do
    rev=$(( $rev * 10 + $temp % 10 ))
    temp=$(( $temp / 10 ))
done

if (( $1 == rev )); then
    echo "Number is palindrome"
else
    echo "Number is not palindrome"
fi