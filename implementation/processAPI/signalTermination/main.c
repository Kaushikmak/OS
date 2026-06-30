#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main() {
    int pid = fork();
    
    if (pid < 0) {
        perror("fork");
        exit(1);
    } else if (pid == 0) {
        printf("Child process running. About to divide by zero...\n");
        // This will cause a SIGFPE (Floating-Point Exception) signal
        int x = 1;
        int y = 0;
        int z = x / y;
        
        // This line will never be reached
        printf("Result: %d\n", z);
        exit(0);
    } else {
        int status;
        waitpid(pid, &status, 0);
        
        // MACRO: WIFSIGNALED(status)
        // Evaluates to true if the child process was terminated by a signal.
        // The lower 7 bits contain the signal number (and optionally a core dump flag in the 8th bit).
        // It is signaled if the lower 7 bits are not 0 (normal exit) and not 0x7f (stopped).
        int signaled = (((signed char) (((status) & 0x7f) + 1) >> 1) > 0);
        // A simpler check for our purposes:
        int simple_signaled = ((status & 0x7f) != 0) && ((status & 0x7f) != 0x7f);
        
        if (simple_signaled) {
            // MACRO: WTERMSIG(status)
            // Evaluates to the number of the signal that caused the termination.
            int term_sig = status & 0x7f;
            printf("Parent: Child process crashed and was terminated by signal %d.\n", term_sig);
        } else {
            printf("Parent: Child exited normally.\n");
        }
    }
    
    return 0;
}
