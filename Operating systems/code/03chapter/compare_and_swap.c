int compare_and_swap(int *value, int expected, int new_value) {
    int temp = *value;
    if (*value == expected)
        *value = new_value;
    return temp;
}

// Example of usage
while (true) {
    waiting[i] = true;
    key = 1;
    while (waiting[i] && key == 1)
        key = compare_and_swap(&lock, 0, 1);
    waiting[i] = false;
    /* Critical section */
    j = (i + 1) % n;
    // Find next process in line that requested access
    while ((j != i) && !waiting[j])
        j = (j + 1) % n;
    if (j == i)
        // Release the lock for everybody
        lock = 0;
    else
        // Give lock to next process in line that requested access
        waiting[j] = false;
    /* End of critical section */
}