#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

void customSignalhandler(int signalNumber){
    printf("\nTHIS IS SIGNAL HANDLER\n");
    printf("User pressed CONTROL + C\n");
    printf("Received signal %d (%s)\n",signalNumber, strsignal(signalNumber));
    exit(1);
}

int main() {
    // SIGINT => ctrl + c
    // SIGTSTP => ctrl + z
    // for more signal number on UNIX run `kill -l` to list all signal and their numbers

    int signalNumber = SIGINT;
    printf("\n\nsignal sent is\t%s\n\n",strsignal(signalNumber));
    signal(signalNumber, customSignalhandler);

    while (1) {
        printf("running...\n");
        sleep(1);
    }

    return 0;
}


