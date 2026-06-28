#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>

int main() {
    int pid = fork();
    if (pid < 0) {
        perror("fork");
        exit(1);
    }
    if (pid == 0) {
        printf("Child before exec\n");
        char *args[] = {"./child", NULL};
        execvp(args[0], args);
        //reached only if exec fails
        perror("execvp");
        exit(1);
    }
    int status;
    // get return value from child process
    wait(&status);
    // raw status code
    printf("Raw status = %d\n", status);
    // kernal stores the exit code in upper byte that's why we have to drop the first 8 bits
    printf("Exit code  = %d\n", status >> 8);
    // other exit codes
    printf("OS code = %d\n",status&0x7F);
    printf("OS code = %s\n",strsignal(status&0x7F));

    return 0;
}