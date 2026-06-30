#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main() {
    int pid = fork();
    if (pid < 0) {
        perror("fork failed");
        exit(1);
    } else if (pid == 0) {
        // Child process
        printf("Child process running. Exiting with code 42.\n");
        exit(42);
    } else {
        // Parent process
        int status;
        waitpid(pid, &status, 0);
        
        // MACRO: WIFEXITED(status)
        // Checks if the child terminated normally.
        // The lower 7 bits of status are 0 if it exited normally.
        int if_exited = (status & 0x7F) == 0;
        
        if (if_exited) {
            // MACRO: WEXITSTATUS(status)
            // Evaluates to the least significant 8 bits of the return code of the child.
            int exit_status = (status & 0xFF00) >> 8;
            printf("Parent detected child exited normally with code: %d\n", exit_status);
        } else {
            printf("Parent detected child did not exit normally.\n");
        }
    }
    
    return 0;
}
