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

/*
After a child process terminates, the kernel returns a raw status code to the parent via wait().

    int status;
    wait(&status);

Traditional layout:

15          8 7          0
+-------------+-----------+
| exit status | signal    |
+-------------+-----------+

Normal Exit
-----------

Child:

    exit(10);

Kernel stores:

    status = 10 << 8
           = 2560

Binary:

    00001010 00000000
    ^^^^^^^^
    exit code

Extract:

    status >> 8    -> 10
    status & 0x7F  -> 0

Signal Termination
------------------

Child:

    int *p = NULL;
    *p = 42;       // SIGSEGV

Kernel stores:

    status = 11

Binary:

    00000000 00001011
                 ^^^^
                signal 11

Extract:

    status >> 8    -> 0
    status & 0x7F  -> 11

Common Signals
--------------

2   -> SIGINT   (Ctrl+C)
9   -> SIGKILL
11  -> SIGSEGV  (Segmentation Fault)
15  -> SIGTERM

Get Signal Description
----------------------

    int sig = status & 0x7F;
    printf("%s\n", strsignal(sig));

Examples:

    11 -> "Segmentation fault"
    9  -> "Killed"
    2  -> "Interrupt"

Shell Exit Codes
----------------

Shell convention:

    exit_code = 128 + signal_number

Examples:

    SIGINT  (2)  -> 130
    SIGKILL (9)  -> 137
    SIGSEGV (11) -> 139
    SIGTERM (15) -> 143

Useful Decoding
---------------

Normal exit code:

    status >> 8

Signal number:

    status & 0x7F

Remember:
- exit(n) -> exit code stored in upper byte
- signal termination -> signal number stored in lower bits
- Raw status is NOT the same as the exit code

*/