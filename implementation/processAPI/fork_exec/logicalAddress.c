#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main(){
    int a = 67;
    int pid = fork();
    if(pid < 0){
        fprintf(stderr, "fork failed\n");
        exit(1);
    }else if(pid == 0){
        a += 3;
        printf("This is child\n");
        printf("value of a: %d\n", a);
        printf("address of a: %p\n", &a);
    }else{
        a -= 7;
        printf("This is parent\n");
        printf("value of a: %d\n", a);
        printf("address of a: %p\n", &a);
    }
    printf("\n");
    return 0;
}
