#/usr/bin/env bash
# Ask the user for two integers a and b. Check if a is greater than b.

check_number() {
    [[ $1 =~ ^[+-]?[0-9]+$ ]] && [[ $2 =~ ^[+-]?[0-9]+$ ]]
}

read -p "Enter first number: " n1
read -p "Enter second number: " n2

if ! $( check_number $n1 $n2 ); then
    echo "The values you inserted are not numbers!"
    exit
fi

if (( $n1 > $n2 )); then
    echo "$n1 > $n2"
elif (( $n2 > $n1 )); then
    echo "$n2 < $n1"
else
    echo "$n1 = $n2"
fi