#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

int main() {
    int pipefd[2];
    
    // pipefd[0] is the read end
    // pipefd[1] is the write end
    
    if (pipe(pipefd) == -1) {
        perror("pipe failed");
        exit(1);
    }
    
    int pid = fork();
    if (pid < 0) {
        perror("fork failed");
        exit(1);
    } else if (pid == 0) {
        // Child process
        // Close the read end since child will write
        close(pipefd[0]);
        
        const char *msg = "Hello from the child process!\n";
        write(pipefd[1], msg, strlen(msg));
        
        // Close the write end after writing
        close(pipefd[1]);
        exit(0);
    } else {
        // Parent process
        // Close the write end since parent will read
        close(pipefd[1]);
        
        char buffer[100];
        int bytes_read = read(pipefd[0], buffer, sizeof(buffer) - 1);
        if (bytes_read > 0) {
            buffer[bytes_read] = '\0';
            printf("Parent received: %s", buffer);
        }
        
        // Close the read end after reading
        close(pipefd[0]);
        
        wait(NULL);
    }
    
    return 0;
}
