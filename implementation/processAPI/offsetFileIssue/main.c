#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <sys/wait.h>

int main() {
    // Demonstration of independent offsets when opening the same file twice
    int fd1 = open("offset_issue.txt", O_CREAT | O_WRONLY | O_TRUNC, S_IRWXU);
    int fd2 = open("offset_issue.txt", O_WRONLY);
    
    if (fd1 < 0 || fd2 < 0) {
        perror("open failed");
        exit(1);
    }
    
    const char *msg1 = "AAAAAAAAAA\n";
    const char *msg2 = "BBBBB\n";
    
    // Writing to fd1
    write(fd1, msg1, strlen(msg1));
    
    // Writing to fd2 without seeking - since fd2 is an independent open description,
    // its offset starts at 0, so it will overwrite the beginning of what fd1 wrote.
    write(fd2, msg2, strlen(msg2));
    
    close(fd1);
    close(fd2);
    
    printf("Check offset_issue.txt. It will contain 'BBBBB\\nAAAA\\n' instead of both full strings sequentially because of independent file offsets.\n");
    return 0;
}
