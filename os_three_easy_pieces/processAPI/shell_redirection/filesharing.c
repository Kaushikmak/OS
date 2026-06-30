#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>

int main() {
    int fd = open("data.txt", O_RDONLY);

    if (fd < 0) {
        perror("open");
        return 1;
    }

    pid_t pid = fork();

    char ch;

    if (pid == 0) {
        // Child
        read(fd, &ch, 1);
        printf("Child read: %c\n", ch);
    }
    else {
        // Parent
        read(fd, &ch, 1);
        printf("Parent read: %c\n", ch);

        wait(NULL);
    }

    close(fd);
    return 0;
}