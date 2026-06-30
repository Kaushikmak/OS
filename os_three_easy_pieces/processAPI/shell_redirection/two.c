#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <stdlib.h>

int main(void) {
    char line[256];

    while (1) {
        printf("$ ");
        fflush(stdout);

        // Get input
        if (fgets(line, sizeof(line), stdin) == NULL) {
            break;
        }

        // Remove trailing newline
        line[strcspn(line, "\n")] = '\0';

        if (strcmp(line, "exit") == 0) {
            break;
        }

        char *command = strtok(line, " ");
        char *symbol  = strtok(NULL, " ");
        char *infile  = strtok(NULL, " ");

        if (command == NULL ||
            symbol == NULL ||
            infile == NULL ||
            strcmp(symbol, "into") != 0) {

            printf("Usage: command into filename\n");
            continue;
        }

        pid_t pid = fork();

        if (pid == 0) {
            // Child: redirect stdin from the file

            close(STDIN_FILENO);

            if (open(infile, O_RDONLY) < 0) {
                perror("open");
                exit(1);
            }

            char *args[] = {command, NULL};

            execvp(command, args);

            // Reached only if execvp() fails
            perror("execvp");
            exit(1);
        }

        // Parent waits for child
        int statusCode;
        wait(&statusCode);

        printf("Child exited with status code: %d\n",
               WEXITSTATUS(statusCode));
    }

    return 0;
}