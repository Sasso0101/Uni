bool test_and_set (bool *target) {
    bool rv = *target;
    *target = true;
    return rv;
}

// Example of usage
do {
    while (test_and_set(&lock)); /* do nothing */
    /* critical section */
    lock = false;
    /* remainder section */
} while (true);