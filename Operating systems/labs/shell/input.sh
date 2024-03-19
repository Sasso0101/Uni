#!/bin/bash
# -n reads whole line
echo -n "Input 1: "
read input1
echo "You wrote: $input1"

echo -n "Input 2: "
# Reads one character
read -n 1 input2
echo "You wrote: $input2"

# One-liner
read -p "Input 3: " input3
echo "You wrote: $input3"

# Default
echo -en "What is your name? "
read myname
echo "Your name is: ${myname:-John Doe}"
