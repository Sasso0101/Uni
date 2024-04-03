// rw_mutex - lock to modify the data
// mutex - lock required to modify counter of readers

// Writer
while (true) {
    lock(rw_mutex);
    write();
    unlock(rw_mutex);
}

// Reader
while (true) {
    wait(mutex);
    read_count++;
    // Check if I'm the first reader and lock r/w access
    // To do so I have to be sure to be the only one, thus the need for the mutex lock
    if (read_count == 1) {
        lock(rw_mutex);
        unlock(mutex);
    }
    read()
    lock(mutex);
    read_count--;
    // Check if I'm the last reader and unlock r/w access
    if (read_count == 0) {
        unlock(rw_mutex);
        unlock(mutex);
    }
}