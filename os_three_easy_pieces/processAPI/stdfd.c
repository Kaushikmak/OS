#include <stdio.h>
#include <unistd.h>

int main() {
    char buf[100];
    // write to std output
    write(1, "$ ", 2);
    char ch = getchar();
    if (ch == 'x') {
        printf("quitting...\n");
        // using read sys call to read from keyboard not just files
        // everything is file in unix!!
        size_t n = read(0, buf, sizeof(buf));
        for (int i = 0; i < n; i++) {
            printf("%c", buf[i]);
        }
    }
    printf("\nch = %c\n", ch);
    return 0;
}