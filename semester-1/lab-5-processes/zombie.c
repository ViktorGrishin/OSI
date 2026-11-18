#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int global_var = 100;

int main() {
    int local_var = 10;

    printf("Parent: Addresses: global = %p, local = %p\n", (void*)&global_var, (void*)&local_var);
    printf("Parent: Values: global = %d, local = %d\n", global_var, local_var);

    printf("Parent: PID = %d\n", getpid());

    getchar();
    pid_t pid = fork();

    if (pid < 0) {
        perror("fork failed");
        exit(1);
    }

    if (pid == 0) { // Код дочернего процесса
        printf("\nChild Process\n");
        printf("Child: PID = %d, PPID = %d\n", getpid(), getppid());
        printf("Child: Same addresses: global = %p, local = %p\n", (void*)&global_var, (void*)&local_var);

        // Изменяем переменные
        printf("Change vars:\n");
        global_var = 200;
        local_var = 20;
        printf("Child: New values: global = %d, local = %d\n", global_var, local_var);
        printf("Child: Values addresses: global = %p, local = %p\n", (void*)&global_var, (void*)&local_var);

        printf("Child: work done!\n");
        exit(5); // Завершаемся с кодом 5
    } else {
	printf("[Parent] Waiting symbal");
        getchar(); // Даем дочернему процессу время поработать
        printf("\nParent Process\n");
        printf("Parent: My values: global = %d, local = %d\n", global_var, local_var);
        printf("Parent: Values addresses: global = %p, local = %p\n", (void*)&global_var, (void*)&local_var);

        // Смотрим, что ребёнок стал zombie
        getchar();
    }
    return 0;
}
