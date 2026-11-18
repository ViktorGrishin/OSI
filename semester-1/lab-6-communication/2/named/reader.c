#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <signal.h>

#define FIFO_NAME "lab6_fifo"

volatile sig_atomic_t keep_running = 1;

void handle_sigint(int sig) {
    keep_running = 0;
}

int main() {
    signal(SIGINT, handle_sigint);

    printf("Читатель: Ожидание подключения писателя...\n");
    
    // Открытие FIFO на чтение БЛОКИРУЕТСЯ, пока писатель не откроет его на запись
    int piped = open(FIFO_NAME, O_RDONLY);
    if (piped == -1) {
        perror("Ошибка открытия FIFO. Убедитесь, что писатель запустил канал");
        exit(1);
    }
    printf("Читатель: Соединение установлено. Начинаем проверку...\n");

    unsigned int received;
    unsigned int expected = 0;
    int first_run = 1;

    while (keep_running) {
        ssize_t bytes_read = read(piped, &received, sizeof(received));
        
        if (bytes_read == 0) {
            printf("\nЧитатель: Писатель закрыл канал (EOF).\n");
            break;
        }
        if (bytes_read == -1) {
            perror("Ошибка чтения");
            break;
        }

        if (first_run) {
            expected = received;
            first_run = 0;
        }

        if (received != expected) {
            printf("Сбой последовательности! Ожидалось: %u, Получено: %u\n", expected, received);
        }
        expected = received + 1;
    }

    printf("Читатель: Завершение работы.\n");
    close(piped);
    return 0;
}
