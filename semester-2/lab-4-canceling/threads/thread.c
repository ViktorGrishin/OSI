#define _GNU_SOURCE
#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

void cleanup_handler(void *arg) {
    char *str = (char *)arg;
    printf("Freeing memory\n");
    free(str);
}

void *mythread(void *arg) {
    char *my_str = (char *)malloc(20 * sizeof(char));
    if (!my_str) return NULL;
    strcpy(my_str, "hello world");
    pthread_cleanup_push(cleanup_handler, my_str);
    int i = 0;
    while (1) {
        printf("mythread: %s, time: %d\n", my_str, ++i);
        sleep(1);
    }

    pthread_cleanup_pop(1); 
    return NULL;
}

int main() {
    pthread_t tid;
    void *retval;

    pthread_create(&tid, NULL, mythread, NULL);
    sleep(3);

    printf("main: Canceling thread...\n");
    pthread_cancel(tid);
    
    pthread_join(tid, &retval);
    
    if (retval == PTHREAD_CANCELED) {
        printf("main: Thread canceled\n");
    }

    return 0;
}