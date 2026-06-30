#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <sys/stat.h>

int main() {
    int fd = open("test_dup.txt", O_CREAT | O_WRONLY | O_TRUNC, S_IRWXU);
    if (fd < 0) {
        perror("open failed");
        exit(1);
    }
    
    // Duplicate the file descriptor
    int new_fd = dup(fd);
    if (new_fd < 0) {
        perror("dup failed");
        exit(1);
    }
    
    const char *msg1 = "Writing to original fd\n";
    write(fd, msg1, strlen(msg1));
    
    const char *msg2 = "Writing to duplicated fd\n";
    write(new_fd, msg2, strlen(msg2));
    
    printf("Original fd: %d, Duplicated fd: %d\n", fd, new_fd);
    
    close(fd);
    close(new_fd);
    
    // Demonstrate dup2: Redirecting standard output
    int out_fd = open("test_dup2.txt", O_CREAT | O_WRONLY | O_TRUNC, S_IRWXU);
    if (out_fd < 0) {
        perror("open for dup2 failed");
        exit(1);
    }
    
    // int STDOUT_FILENO_MACRO = 1; // MACRO: STDOUT_FILENO
    // Instead of using STDOUT_FILENO macro, we use its value 1
    int stdout_fd = 1;
    
    if (dup2(out_fd, stdout_fd) < 0) {
        perror("dup2 failed");
        exit(1);
    }
    
    // This will be written to test_dup2.txt instead of terminal
    printf("This message is printed to standard output, but redirected via dup2!\n");
    
    close(out_fd);
    return 0;
}
