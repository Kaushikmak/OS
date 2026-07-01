#include <stdio.h>
#include <sys/signal.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>

enum CUSTOMSTATUS{
    FORK_DIDNT_HAPPENED = 0,
    EVERYTHING_OKAY = 1,
};

const char *stringExit(int exitCode) {
    switch (exitCode) {
        case FORK_DIDNT_HAPPENED:
            return "FORK_DIDNT_HAPPENED";
        case EVERYTHING_OKAY:
            return "EVERYTHING_OKAY";
        default:
            return "UNKNOWN_EXIT_CODE";
    }
}

int main(){
    pid_t pid = fork();
    if(pid < 0){
        perror("fork");
        exit(FORK_DIDNT_HAPPENED);
    }else if(pid == 0){
        printf("Child process running\n");
        int x = 5;
        int y = 0;
        int z = x/y;
        printf("result of division of x/y: %d\n",z);
        exit(EVERYTHING_OKAY);
    }else{
        int captureStatus;
        wait(&captureStatus);
        int exitCode = (captureStatus >> 8);
        int signal = (captureStatus & 0x7F);
        int coreDumped = (captureStatus & 0x80);
        if(signal == 0){
            printf("child exited with exitcode: %d ",exitCode);
            printf("(%s)\n",stringExit(exitCode));
        }else{
            printf("child exited with signal: %d ",signal);
            printf("(%s)\n",strsignal(signal));
            printf("Core dumped: %s\n",coreDumped ? "Yes" : "No");
        }
    }

    return 0;
}