#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <limits.h>
#include <signal.h>
#include <errno.h>

#define FIFO_NAME "lab6_fifo"

volatile sig_atomic_t keep_running = 1;

void handle_sigint(int sig) {
    keep_running = 0;
}

int main() {
    // Игнорируем SIGPIPE, чтобы ядро не убивало процесс аварийно при закрытии читателя.
    // Вместо этого вызов write() вернет -1, а errno установится в EPIPE.
    signal(SIGPIPE, SIG_IGN);
    signal(SIGINT, handle_sigint);

    // Создаем именованный канал (FIFO) с правами доступа 0666
    if (mkfifo(FIFO_NAME, 0666) == -1) {
        if (errno != EEXIST) { // Если канал уже существует — это не ошибка
            perror("Ошибка создания FIFO");
            exit(1);
        }
    }

    printf("Писатель: Ожидание подключения читателя...\n");
    
    // Открытие FIFO на запись БЛОКИРУЕТСЯ, пока читатель не откроет его на чтение
    int piped = open(FIFO_NAME, O_WRONLY);
    if (piped == -1) {
        perror("Ошибка открытия FIFO");
        unlink(FIFO_NAME);
        exit(1);
    }
    printf("Писатель: Читатель подключился. Начинаем передачу...\n");

    unsigned int counter = 0;

    while (keep_running) {
        // Записываем число в pipe
        if (write(piped, &counter, sizeof(counter)) == -1) {
            if (errno == EPIPE) {
                printf("\nПисатель: Читатель отключился (Обрыв канала / EPIPE).\n");
                break;
            }
            perror("Ошибка записи");
            break;
        }
        
        counter++;
        if (counter == UINT_MAX) counter = 0;
    }

    printf("Писатель: Освобождение ресурсов и удаление FIFO.\n");
    close(piped);
    unlink(FIFO_NAME); // Высвобождаем pipe
    return 0;
}
