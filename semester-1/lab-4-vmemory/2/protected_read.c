#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/mman.h>


int main() {
    printf("PID: %d\n", getpid());

    printf("allocate mmap region\n");
    size_t page_size = getpagesize();
    char *region = mmap(NULL, 10 * page_size, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);

    printf("Write to region\n");
    region[0] = 'a';
    printf("Reading %c\n", region[0]);

    printf("mmap region protected for read\n");
    mprotect(region, page_size, PROT_READ);
    getchar();
    printf("Reading %c\n", region[0]);
    getchar();
    printf("Writing\n");
    region[0] = 'a';


    return 0;
}
