#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int global_var = 100;

int main() {
    int local_var = 10;

    printf("[Grandparent] Starts!\n");


    getchar();
    pid_t ppid = fork();

    if (ppid < 0) {
        perror("fork1 failed\n");
        exit(-1);
    }

    if (ppid == 0) { // Код родительского процесса
        printf("\n[Parent] starts\n");
        printf("[Parent] PID = %d, PPID = %d\n", getpid(), getppid());
        printf("[Parent] Fork\n");

        pid_t pid = fork();
        if (pid < 0) {
            perror("fork2 failed\n");
            exit(-1);
        }

        if (pid == 0){
            printf("\n[Child] PID = %d, PPID = %d\n", getpid(), getppid());
            printf("[Child] starts\n");
            printf("[Child] Sleep 30\n");
            sleep(30);
            printf("PPID = %d\n", getppid());
            exit(0);
        } else
        {
            printf("[Parent] Child is %d\n", pid);
            printf("[Parent] Sleep 10\n");
            sleep(10);
            printf("[Parent] End\n");
            exit(0);
        }

        exit(0);
    } else {
        printf("[Grandparent] Parent is %d\n", ppid);
	printf("[Grandparent] Waiting to Enter\n");
       	getchar();
        printf("[Grandparent] Ended without checking parent exit code\n");
    }
    return 0;
}
