// Full - semaphore with value of free slots in buffer
// Empty - semaphore with value of elements present in the buffer
// Mutex - locks access to buffer to one process only

full <- number of slots in buffer
empty <- 0

// Producer
while (true) {
    wait(full); // If full = 0 wait, otherwise decrement full by 1 and continue
    lock(mutex); // If someone is reading/writing wait, otherwise continue
    produce();
    unlock(mutex); // Release access of buffer
    signal(empty); // Increment empty by 1
}

// Consumer
while (true) {
    wait(empty); // If empty = 0 wait, otherwise decrement empty by 1 and continue
    lock(mutex); // If someone is reading/writing wait, otherwise continue
    consume();
    unlock(mutex); // Release access of buffer
    signal(full); // Increment full by 1
}