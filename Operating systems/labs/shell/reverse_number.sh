#/usr/bin/env bash

# if (( $# < 1 )); then
#     echo "Invalid data"
# else
#     echo "$1" | rev
# fi


if (( $# < 1 )); then
    echo "Invalid data"
else
    n=$1
    while (( n != 0 )); do
        echo -n "$(( n % 10 ))"
        n=$(( n / 10 ))
    done
fi