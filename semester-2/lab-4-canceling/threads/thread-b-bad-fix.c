#define _GNU_SOURCE
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>

void *mythread(void *arg) {
    pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, NULL);
    int i = 0;
    while (1) ++i;
    return NULL;
}

int main() {
    pthread_t tid;
    void *retval;

    printf("main: Creating thread...\n");
    pthread_create(&tid, NULL, mythread, NULL);

    sleep(3);

    printf("main: Canceling thread...\n");
    pthread_cancel(tid);

    pthread_join(tid, &retval);
    
    if (retval == PTHREAD_CANCELED) {
        printf("main: Thread was successfully canceled!\n");
    } else {
        printf("main: Thread exited normally.\n");
    }

    return 0;
}