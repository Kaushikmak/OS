#include <stdio.h>
#include <unistd.h>

int main() {
    char buf[100];
    
    // Instead of using printf, we use the write() system call directly
    // 1 is the file descriptor for standard output (STDOUT_FILENO)
    write(1, "Enter some text: ", 17);
    
    // Instead of using scanf or fgets, we use the read() system call directly
    // 0 is the file descriptor for standard input (STDIN_FILENO)
    size_t n = read(0, buf, sizeof(buf));
    
    write(1, "You entered: ", 13);
    
    // Write back exactly what was read
    write(1, buf, n);
    
    return 0;
}
