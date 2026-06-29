// ls > output.txt
// redirects stdout of ls to output.txt

// close(1)
// //so now this fd is empty
// open(foo,write only)
// //since we know os assigns first available fd so open is assigned to fd 1
// exec(ls)
// //ls will print it's output to stdout normalls that is fd 1
// //but it's now redirected to file foo now

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>


// customs implementation of > 
// using `goto`
int main(void)
{
    char line[256];

    while (1) {
        printf("$ ");
        fflush(stdout); // flush is now
        //if is fgets return null break the program
        if (fgets(line, sizeof(line), stdin) == NULL)
            break;

        // remove newline and replace it with \0
        line[strcspn(line, "\n")] = '\0';

        // if exit command break
        if (strcmp(line, "exit") == 0)
            break;

        // search for first space
        char *command = strtok(line, " ");
        // search for custom command
        char *symbol  = strtok(NULL, " ");
        // outputfile
        char *outfile = strtok(NULL, " ");
        // check semantics
        if (command == NULL || symbol == NULL || outfile == NULL || strcmp(symbol, "goto") != 0) {
            printf("Usage: command `goto` filename\n");
            continue;
        }

        // fork it
        pid_t pid = fork();

        if (pid == 0) {
            // child process
            // close stdout i.e. 1 or in c STDOUT_FILENO
            close(1);
            // Open file for writing.
            // If it doesn't exist, create it
            // If it already exists, truncate (erase) its contents
            open(outfile,O_WRONLY | O_CREAT | O_TRUNC,0644);
            // Build the argument list for execvp().
            // args[0] is the program name.
            // The array must end with NULL.
            char *args[] = {command, NULL};
            // Execute the command.
            // execvp() searches the PATH for the executable
            // and replaces the current process with it.
            execvp(command, args);
            // if any error occurs exit with custom exitcode 1
            perror("execvp");
            exit(1);
        }
        // parent will wait for it's child to exit
        int statuscode;
        wait(&statuscode);
        printf("Child exited with status code: %d\n",statuscode>>8);
    }

    return 0;
}