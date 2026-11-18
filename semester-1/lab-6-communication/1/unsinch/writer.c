#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>
#include <limits.h>

#define SHM_NAME "lab6_unsinchr_shared_memory"
#define PAGE_SIZE 4096

int main() {
    int fd = open(SHM_NAME, O_CREAT | O_RDWR, 0666);
    ftruncate(fd, PAGE_SIZE);
    unsigned int *buffer = mmap(NULL, PAGE_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    
    unsigned int counter = 0;
    int max_elements = PAGE_SIZE / sizeof(unsigned int);

    while (1) {
        for (int i = 0; i < max_elements; i++) {
            buffer[i] = counter++;
            if (counter == UINT_MAX) counter = 0;
        }
	sleep(10);
	printf("New iteration");
    }
    return 0;
}
