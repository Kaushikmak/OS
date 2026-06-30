#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>

int main() {

    pid_t pid = fork();

    char ch;

    if (pid == 0) {
        int fd = open("data.txt", O_RDONLY);
        read(fd, &ch, 1);
        printf("Child: %c\n", ch);
        close(fd);
    }
    else {
        int fd = open("data.txt", O_RDONLY);
        read(fd, &ch, 1);
        printf("Parent: %c\n", ch);
        close(fd);
        wait(NULL);
    }
}