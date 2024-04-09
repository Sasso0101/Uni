#/usr/bin/env bash
curr_time=$(date +"%H")
if [[ curr_time -ge 5 && curr_time -le 12 ]]; then
    echo "Good morning!"
elif [[ curr_time -ge 12 && curr_time -le 18 ]]; then
    echo "Good afternoon!"
else
    echo "Good night!"
fi