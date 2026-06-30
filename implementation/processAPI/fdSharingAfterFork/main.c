#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/stat.h>

int main() {
    // Open a file before fork
    // MACRO equivalents:
    // O_CREAT: create file if it doesn't exist
    // O_WRONLY: write only
    // O_TRUNC: truncate size to 0
    // S_IRWXU: user has read, write, execute permissions
    int fd = open("shared_file.txt", O_CREAT | O_WRONLY | O_TRUNC, S_IRWXU);
    if (fd < 0) {
        perror("open failed");
        exit(1);
    }
    
    int pid = fork();
    if (pid < 0) {
        perror("fork failed");
        exit(1);
    } else if (pid == 0) {
        // Child process
        const char *child_msg = "Child writing to shared file descriptor\n";
        write(fd, child_msg, strlen(child_msg));
        printf("Child (PID: %d) wrote to fd %d\n", (int)getpid(), fd);
    } else {
        // Parent process
        // wait(NULL) macro equivalent, wait for child
        wait(NULL);
        
        const char *parent_msg = "Parent writing to shared file descriptor\n";
        write(fd, parent_msg, strlen(parent_msg));
        printf("Parent (PID: %d) wrote to fd %d\n", (int)getpid(), fd);
    }
    
    close(fd);
    return 0;
}
