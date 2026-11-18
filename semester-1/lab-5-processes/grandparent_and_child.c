#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/prctl.h>

int main() {
    printf("[Grandparent] Starts!\n");
    getchar();
    if (prctl(PR_SET_CHILD_SUBREAPER, 1) == -1) exit(1);
    printf("[grandparent] can take child, if parent is zombie"); 
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
