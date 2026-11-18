#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/mman.h>


int main() {
    printf("PID: %d\n", getpid());

    printf("allocate mmap region\n");
    size_t page_size = getpagesize();
    char *region = mmap(NULL, 10 * page_size, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);

    getchar();
    mprotect(region, page_size, PROT_NONE);
    printf("mmap region protected for all\n");
    char c = region[0];

    return 0;
}
