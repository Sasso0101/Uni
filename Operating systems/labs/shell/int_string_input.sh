#!/bin/bash
# Write a shell script that takes two parameters and behaves as follows:
# if both are numbers, it sums them and prints result
# if both are strings, it concats them and prints result
# if the first is number and other is not, it outputs the first parameter
# if the first is string and the second is number, it checks if the string starts with a number: if yes, it echoes "OS".
# Tip: you can check if a string contains a number by using regexes and the =~ notation

is_number() {
    [[ $1 =~ ^[0-9]+$ ]]
}

if (( $# < 2 )); then
    echo "Not enough parameters!"
    exit
fi

if $(is_number $1) && $(is_number $2); then
    echo "Sum: $(( $1 + $2 ))"
elif ! $(is_number $1) && ! $(is_number $2); then
    echo "Concatenation: $1$2"
elif $(is_number $1) && ! $(is_number $2); then
    echo "$(( $1 ))"
elif ! $(is_number $1) && $(is_number $2); then
    echo "OS"
fi