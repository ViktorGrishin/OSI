#include <stdio.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
    printf("PID: %d\n", getpid());
    getchar();
    execv(argv[0], argv);
    printf("Hello world\n");
    return 0;
}