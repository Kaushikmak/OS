#include <stdio.h>
#include <stdlib.h>

int main(){
    printf("\n\nthis is program named child\nHello world\n\n");
    // exit with status code of 10
    int y = 6/0;
    int buf[2] ={0};
    int x = buf[300000];
    printf("x: %d",x);
    exit(10);
    return 0;
}