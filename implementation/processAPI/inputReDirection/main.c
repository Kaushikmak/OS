#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/stat.h>

int main() {
    // Create a dummy file with some text
    int temp_fd = open("input.txt", O_CREAT | O_WRONLY | O_TRUNC, S_IRWXU);
    if (temp_fd < 0) {
        perror("open failed");
        exit(1);
    }
    const char *text = "line1\nline2\nline3\n";
    write(temp_fd, text, strlen(text));
    close(temp_fd);

    int pid = fork();
    if (pid < 0) {
        perror("fork failed");
        exit(1);
    } else if (pid == 0) {
        // Child process
        // 0 is standard input
        close(0); // MACRO: STDIN_FILENO
        
        int fd = open("input.txt", O_RDONLY);
        if (fd < 0) {
            perror("open for read failed");
            exit(1);
        }
        
        // The newly opened file descriptor will be 0 (STDIN_FILENO)
        // Now if we exec 'wc -l', it will count the lines in standard input,
        // which is now reading from input.txt
        
        char *args[] = {"wc", "-l", NULL};
        execvp(args[0], args);
        perror("execvp failed");
        exit(1);
    } else {
        // Parent process
        // wait(NULL) macro equivalent, wait for child
        wait(NULL);
        printf("Parent: Child finished input redirection.\n");
    }
    
    return 0;
}
