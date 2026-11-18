#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>

#define SHM_NAME "lab6_unsinchr_shared_memory"
#define PAGE_SIZE 4096

int main() {
    int fd = open(SHM_NAME, O_RDWR);
    unsigned int *buffer = mmap(NULL, PAGE_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    
    unsigned int expected = buffer[0];
    int max_elements = PAGE_SIZE / sizeof(unsigned int);
    int count_error = 0;
    while (1) {
        for (int i = 0; i < max_elements; i++) {
            if (buffer[i] != expected) {
                printf("Сбой последовательности! Ожидалось: %u, Получено: %u\n", expected, buffer[i]);
                expected = buffer[i];
		++count_error;
            }
            expected++;
        }
	if (count_error > 15)
		break;
    }
    return 0;
}
