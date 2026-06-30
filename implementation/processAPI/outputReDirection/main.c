#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <sys/stat.h>

int main() {
    int pid = fork();
    if (pid < 0) {
        perror("fork failed");
        exit(1);
    } else if (pid == 0) {
        // Child process
        // 1 is standard output
        close(1); // MACRO: STDOUT_FILENO
        
        int fd = open("output.txt", O_CREAT | O_WRONLY | O_TRUNC, S_IRWXU);
        if (fd < 0) {
            perror("open failed");
            exit(1);
        }
        
        // The newly opened file descriptor will be 1 (STDOUT_FILENO)
        // Now if we exec 'echo', it will write its output to output.txt
        
        char *args[] = {"echo", "This text is redirected to output.txt instead of the screen.", NULL};
        execvp(args[0], args);
        perror("execvp failed");
        exit(1);
    } else {
        // Parent process
        wait(NULL);
        printf("Parent: Child finished output redirection. Check output.txt\n");
    }
    
    return 0;
}
