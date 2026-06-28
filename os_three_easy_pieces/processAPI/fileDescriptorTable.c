#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>

int main(){
    int pid = (int)getpid();
    printf("PID is %d\n",pid);


    // to see fd go to /proc/PID_VALUE/fd/ or fdinfo

    // see next int will be assigned to this fd
    int fd = open("test.txt", O_RDONLY);
    printf("file descriptor is: %d",fd);
    
    getchar();
    return 0;
}