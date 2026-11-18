#define _GNU_SOURCE
#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

void cleanup_handler(void *arg) {
    char *str = (char *)arg;
    printf("Cleanup handler: Freeing memory at %p\n", arg);
    free(str);
}

void *mythread(void *arg) {
    
    char *my_str = NULL;
    thread_cleanup_push(cleanup_handler, my_str);
    my_str  = (char *)malloc(20 * sizeof(char));
    if (!my_str) {pthread_cleanup_pop(1); return NULL;}
    strcpy(my_str, "hello world");

    while (1) {
        printf("mythread: %s\n", my_str);
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
        printf("main: Thread canceled, memory was freed by cleanup handler.\n");
    }

    return 0;
}