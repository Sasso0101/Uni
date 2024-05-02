#include <stdio.h>
#include <signal.h>
#include <unistd.h>

void signal_handler(int signal_number) {
    printf("Received signal: %d\n", signal_number);
}

int main() {
    void (*old_handler)(int);
    printf("I can be CTRL-C\n");
    sleep(3);
    old_handler = signal(SIGINT, SIG_IGN);
    printf("I'm protected from CTRL-C\n");
    sleep(3);
    signal(SIGINT, old_handler);
    printf("I can be CTRL-C\n");
    sleep(3);
    signal(SIGINT, signal_handler);
    printf("Managed by custom function\n");
    sleep(3);
    printf("Bye!\n");
}