// Free - semaphore with value of free slots in buffer
// Empty - semaphore with value of elements present in the buffer
// Mutex - binary semaphore that locks access to buffer to one process only

free <- number of slots in buffer
elements <- 0
mutex <- 1

// Producer
while (true) {
    wait(free); // If free = 0 wait, otherwise decrement free by 1 and continue
    wait(mutex); // If someone is reading/writing wait, otherwise continue
    produce();
    signal(mutex); // Release access of buffer
    signal(elements); // Increment elements by 1
}

// Consumer
while (true) {
    wait(elements); // If elements = 0 wait, otherwise decrement elements by 1 and continue
    wait(mutex); // If someone is reading/writing wait, otherwise continue
    consume();
    signal(mutex); // Release access of buffer
    signal(free); // Increment free by 1
}