#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>
#include <signal.h>
#include <errno.h>

#define SHM_NAME "/tmp/lab6_shm_sync"
#define PAGE_SIZE 4096

volatile sig_atomic_t data_ready = 0;
volatile sig_atomic_t keep_running = 1;
pid_t writer_pid = 0;

void handle_sigusr2(int sig) { data_ready = 1; }
void handle_sigint(int sig) { keep_running = 0; }

int main() {
    signal(SIGUSR2, handle_sigusr2);
    signal(SIGINT, handle_sigint);

    int fd = open(SHM_NAME, O_RDWR);
    if (fd == -1) {
        perror("Не удалось открыть разделяемую память. Запустите сначала писателя");
        exit(1);
    }

    pid_t *pid_buf = mmap(NULL, PAGE_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    unsigned int *data_buf = (unsigned int *)(pid_buf + 2);

    writer_pid = pid_buf[0];
    pid_buf[1] = getpid(); // Регистрируем себя для писателя
    printf("Читатель запущен. PID: %d. Подключение к Писателю (PID: %d)\n", getpid(), writer_pid);

    unsigned int expected = 0;
    int max_elements = (PAGE_SIZE - 2 * sizeof(pid_t)) / sizeof(unsigned int);
    int first_run = 1;
    printf("Start finding I/O error!\n");
    while (keep_running) {
        while (!data_ready && keep_running) {
            pause();
        }
        if (!keep_running) break;
        data_ready = 0;

        if (first_run) {
            expected = data_buf[0];
            first_run = 0;
        }

        for (int i = 0; i < max_elements; i++) {
            if (data_buf[i] != expected) {
                printf("Сбой! Ожидалось: %u, Получено: %u\n", expected, data_buf[i]);
            }
            expected = data_buf[i] + 1;
        }

        // Отправляем сигнал писателю и проверяем, жив ли он
        if (kill(writer_pid, SIGUSR1) == -1) {
            if (errno == ESRCH) {
                printf("\nЗавершение: Писатель (PID %d) прекратил работу.\n", writer_pid);
		break;
            }
        }
    }

    // Если читатель падает сам по SIGINT — уведомляем писателя
    if (!keep_running && kill(writer_pid, 0) == 0) {
        kill(writer_pid, SIGINT);
    }

    printf("Читатель завершил работу.\n");
    munmap(pid_buf, PAGE_SIZE);
    return 0;
}
