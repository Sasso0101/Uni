#/usr/bin/env bash
#Ask the user for two integers (divisor and dividend). Compute the quotient and the remainder.

check_number() {
    [[ $1 =~ ^[+-]?[0-9]+$ ]] && [[ $2 =~ ^[+-]?[0-9]+$ ]]
}

read -p "Enter dividend: " n1
read -p "Enter divisor: " n2

if ! $( check_number $n1 $n2 ); then
    echo "The values you inserted are not numbers!"
    exit
fi

echo "Result: $(( $n1 / $n2 ))"
echo "Remainder: $(( $n1 % $n2 ))"