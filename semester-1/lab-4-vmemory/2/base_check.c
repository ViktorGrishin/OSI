#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/mman.h>

void recursive_func(int depth) {
    char buffer[4096];
    if (depth == 0) {printf("Max rec depth\n") ; getchar(); return;}
    sleep(1);
    recursive_func(depth - 1);
}

int main() {
    printf("PID: %d\n", getpid());
    getchar();
    printf("Stack\n");
    recursive_func(0);
    printf("malloc\n");
    char *ptrs[10];
    for (int i = 0; i < 10; i++) {
        ptrs[i] = malloc(1024 * 1024);
    }
    printf("malloc allocated\n");
    getchar();
    for (int i = 0; i < 10; i++) {
        free(ptrs[i]);
    }
    printf("malloc deallocated\n");
    getchar();
    printf("allocate mmap region\n");
    size_t page_size = getpagesize();
    char *region = mmap(NULL, 1024 * page_size, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    
    getchar();

    printf("mmap region part deallocated\n");
    munmap(region + 3 * page_size, 3 * page_size);
    getchar();



    return 0;
}
