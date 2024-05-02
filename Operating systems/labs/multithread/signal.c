#include <stdio.h>
#include <sys/types.h>
#include <signal.h>

int main() {
    pid_t pid = 97995;

    int result = kill(pid, SIGTERM);

    if (result == 0) {
        printf("Signal successfully sent!\n");
    } else {
        printf("Failed to send signal!\n");
    }
}