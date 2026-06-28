#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(){
    printf("pid of this process is %d\n",(int)getpid());
    int pid = fork();
    if(pid < 0){
        fprintf(stderr, "fork failed\n");
        exit(1);
    }else if(pid == 0){
        printf("pid of child is %d\n",(int)getpid()); 
    }else{
        int wc = wait(NULL);
        printf("pid of child is %d whose parent is %d\n",pid,(int)getpid());
    }
    return 0;
}
