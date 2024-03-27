bool test_and_set (bool *target) {
    bool rv = *target;
    *target = true;
    return rv;
}