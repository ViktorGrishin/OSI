#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main() {
    int piped[2];
    if (pipe(piped) == -1) {
        perror("pipe");
        exit(1);
    }

    pid_t pid = fork();

    if (pid > 0) { // Родитель (Писатель)
        close(piped[0]);
        dup2(piped[1], STDOUT_FILENO);
        close(piped[1]);
	printf("Start writing!\n");
        for (unsigned int i = 0; i < 100000; i++) {
            printf("%u\n", i);
        }
        
        fflush(stdout); 
        wait(NULL); 
    } 
    else if (pid == 0) { // Потомок (Читатель)
        close(piped[1]);
        dup2(piped[0], STDIN_FILENO);
        close(piped[0]);

        unsigned int received;
        unsigned int expected = 0;
        printf("Start reading!\n");
        while (scanf("%u", &received) == 1) {
            if (received != expected) {
                fprintf(stderr, "Сбой! Ожидалось: %u, Получено: %u\n", expected, received);
            }
            expected = received + 1;
        }
        fprintf(stderr, "Успешно прочитано до: %u\n", expected - 1);
    }

    return 0;
}
