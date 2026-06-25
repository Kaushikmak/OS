#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(){
    printf("hello world (pid:%d)\n",(int)getpid());
    int pid = fork();
    if(pid < 0){
        fprintf(stderr, "fork failed\n");
        exit(1);
    }else if(pid == 0){
        printf("hello, this is child process (pid:%d)\n",(int)getpid());
    }else{
        printf("hello this is parent of %d (pid:%d)\n",pid,(int)getpid());
    }
    return 0;
}
