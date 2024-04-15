#/usr/bin/env bash

read -p "Enter number: " n

check_number() {
    [[ $1 =~ ^[+-]?[0-9]+$ ]]
}

is_even() {
    ! (( $1 % 2 ))
}

if ! $( check_number $n ); then
    echo "The value you inserted is not a number!"
    exit
fi

if is_even $n; then
    echo "$n is even!"
else
    echo "$n is odd!"
fi