// rw_mutex - lock to modify the data
// mutex - lock required to modify counter of readers

rw_mutex <- 1
mutex <- 1
read_count <- 0

// Writer
while (true) {
    wait(rw_mutex);
    write();
    signal(rw_mutex);
}

// Reader
while (true) {
    wait(mutex);
    read_count++;
    // Check if I'm the first reader and lock r/w access
    // To do so I have to be sure to be the only one, thus the need for the mutex lock
    if (read_count == 1) {
        wait(rw_mutex);
        signal(mutex);
    }
    signal(mutex);
    read();
    wait(mutex);
    read_count--;
    // Check if I'm the last reader and unlock r/w access
    if (read_count == 0) {
        signal(rw_mutex);
    }
    signal(mutex);
}