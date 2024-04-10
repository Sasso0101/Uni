#!/bin/bash
# Write a shell script toconvert a temperature from C to F and from F to C degrees
# Ctemp = (Ftemp-32)*1.8
# Ftemp = Ctemp*1.8+32
# the script should take two inputs: the number and a letter, either F or C, which indicates if the number is celsius of farenheit
# the script should output the temperature in the other measure unit.
# The student should implement two functions that take a number each, one for C to F and one for F to C 
# Tip: you cant directly edit the $1, $2, ... function vars, you have to copy them into another variable first

if (( $# < 2 )); then
    echo "Not enough parameters"
    exit
fi

c_to_f() {
    echo $(( $1*18/10+32 ))
}

f_to_c() {
    echo $(( ($1-32)*18/10 ))
}

case $2 in
    "C")
        c_to_f $1
        ;;
    "F")
        f_to_c $1
        ;;
    *)
        echo "Not a valid option"
        ;;
esac