#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>
#include <limits.h>
#include <signal.h>
#include <errno.h>

#define SHM_NAME "/tmp/lab6_shm_sync"
#define PAGE_SIZE 4096

volatile sig_atomic_t reader_ready = 0;
volatile sig_atomic_t keep_running = 1;
pid_t reader_pid = 0;

void handle_sigusr1(int sig) { reader_ready = 1; }
void handle_sigint(int sig) { keep_running = 0; }

int main() {
    signal(SIGUSR1, handle_sigusr1);
    signal(SIGINT, handle_sigint);

    int fd = open(SHM_NAME, O_CREAT | O_RDWR, 0666);
    ftruncate(fd, PAGE_SIZE);
    pid_t *pid_buf = mmap(NULL, PAGE_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    unsigned int *data_buf = (unsigned int *)(pid_buf + 2);

    pid_buf[0] = getpid();
    printf("Писатель запущен. PID: %d. Ожидание читателя...\n", getpid());

    // Ожидание инициализации читателя
    while (pid_buf[1] == 0 && keep_running) { sleep(1); }
    reader_pid = pid_buf[1];

    unsigned int counter = 0;
    int max_elements = (PAGE_SIZE - 2 * sizeof(pid_t)) / sizeof(unsigned int);
    printf("Start writing!\n");
    while (keep_running) {
        for (int i = 0; i < max_elements; i++) {
            data_buf[i] = counter++;
            if (counter == UINT_MAX) counter = 0;
        }

        // Отправляем сигнал и проверяем, жив ли читатель
        if (kill(reader_pid, SIGUSR2) == -1) {
            if (errno == ESRCH) {
                printf("\nЗавершение: Читатель (PID %d) прекратил работу.\n", reader_pid);
                break;
            }
        }

        // Ждем отмашки от читателя
        while (!reader_ready && keep_running) {
            pause();
        }
        reader_ready = 0;
    }

    // Если вышли по SIGINT писателя — аккуратно уведомляем читателя, если он жив
    if (!keep_running && kill(reader_pid, 0) == 0) {
        kill(reader_pid, SIGINT);
    }

    printf("Писатель завершил работу. Освобождение ресурсов...\n");
    munmap(pid_buf, PAGE_SIZE);
    unlink(SHM_NAME);
    return 0;
}
