#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>

int main() {
    // Standard file descriptors are 0 (stdin), 1 (stdout), 2 (stderr).
    // Opening a new file should give us fd 3.
    int fd1 = open("dummy1.txt", O_CREAT | O_WRONLY | O_TRUNC, S_IRWXU);
    if (fd1 < 0) {
        perror("open failed");
        exit(1);
    }
    
    int fd2 = open("dummy2.txt", O_CREAT | O_WRONLY | O_TRUNC, S_IRWXU);
    if (fd2 < 0) {
        perror("open failed");
        exit(1);
    }
    
    printf("Standard IN fd: 0\n"); // MACRO: STDIN_FILENO
    printf("Standard OUT fd: 1\n"); // MACRO: STDOUT_FILENO
    printf("Standard ERR fd: 2\n"); // MACRO: STDERR_FILENO
    
    printf("First opened file descriptor: %d\n", fd1);
    printf("Second opened file descriptor: %d\n", fd2);
    
    // Close stdout (fd 1) and open a new file. It should get fd 1.
    close(1);
    int fd3 = open("dummy3.txt", O_CREAT | O_WRONLY | O_TRUNC, S_IRWXU);
    
    // Note: Since stdout is closed and replaced, printf will write to dummy3.txt
    printf("This will be written to dummy3.txt. Its fd is: %d\n", fd3);
    
    close(fd1);
    close(fd2);
    close(fd3);
    
    return 0;
}
