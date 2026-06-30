#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main() {
    char input[] = "ls -l -a /tmp";
    char *args[10]; // Array of string pointers to hold arguments
    int argc = 0;
    
    printf("Original command: '%s'\n", input);
    
    // strtok modifies the original string by replacing delimiters with null characters
    char *token = strtok(input, " \t\n");
    while (token != NULL && argc < 9) {
        args[argc++] = token;
        token = strtok(NULL, " \t\n");
    }
    
    // The exec family of functions requires the arguments array to be null-terminated
    args[argc] = NULL;
    
    printf("Parsed arguments:\n");
    for (int i = 0; i < argc; i++) {
        printf("args[%d]: '%s'\n", i, args[i]);
    }
    
    // In a real shell, we would now fork() and execvp(args[0], args)
    
    return 0;
}
