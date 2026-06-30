#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>

int main(void) {
    char line[256];

    while (1) {
        printf("$ ");
        fflush(stdout);

        if (fgets(line, sizeof(line), stdin) == NULL) {
            break;
        }

        // Remove trailing newline
        line[strcspn(line, "\n")] = '\0';

        if (strcmp(line, "exit") == 0) {
            break;
        }
        
        if (strlen(line) == 0) {
            continue;
        }

        // We will parse the command and look for > or <
        char *args[20];
        int argc = 0;
        char *outfile = NULL;
        char *infile = NULL;

        char *token = strtok(line, " ");
        while (token != NULL && argc < 19) {
            if (strcmp(token, ">") == 0) {
                // Next token should be the output file
                outfile = strtok(NULL, " ");
            } else if (strcmp(token, "<") == 0) {
                // Next token should be the input file
                infile = strtok(NULL, " ");
            } else {
                args[argc++] = token;
            }
            if (outfile == NULL && infile == NULL) {
                token = strtok(NULL, " ");
            } else {
                // We just parsed a redirection file, get next token
                token = strtok(NULL, " ");
            }
        }
        args[argc] = NULL;

        if (argc == 0) continue;

        pid_t pid = fork();

        if (pid < 0) {
            perror("fork");
            continue;
        } else if (pid == 0) {
            // Child process
            
            if (outfile != NULL) {
                // Output redirection
                close(1); // MACRO: STDOUT_FILENO
                if (open(outfile, O_WRONLY | O_CREAT | O_TRUNC, 0644) < 0) {
                    perror("open output file");
                    exit(1);
                }
            }
            
            if (infile != NULL) {
                // Input redirection
                close(0); // MACRO: STDIN_FILENO
                if (open(infile, O_RDONLY) < 0) {
                    perror("open input file");
                    exit(1);
                }
            }

            execvp(args[0], args);
            perror("execvp");
            exit(1);
        }

        // Parent waits for child
        int statuscode;
        wait(&statuscode);

        // MACRO: WEXITSTATUS
        int exit_status = (statuscode & 0xFF00) >> 8;
        printf("Child exited with status code: %d\n", exit_status);
    }

    return 0;
}
