#define _GNU_SOURCE

#define NUM_THREADS 5

#include <stdio.h>
#include <pthread.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <malloc.h>

void *mythread(void *arg) {
	printf("mythread [%d %d %d]: Hello from mythread!\n", getpid(), getppid(), gettid());
	printf("Thread %d:\n\tgettid() = %d\n\t pthread_self() = %ld\n\n", gettid(), gettid(), pthread_self());
	return NULL;
}

int main() {
	int err;
	pthread_t* tid = malloc(NUM_THREADS * sizeof(pthread_t));
	if (!tid){
		puts("main: malloc failure");
	} 

	printf("main [%d %d %d]: Hello from main!\n", getpid(), getppid(), gettid());
	for (int i = 0; i < NUM_THREADS; ++i){ 
		err = pthread_create(&tid[i], NULL, mythread, NULL);
		if (err) {
	    		printf("main: pthread_create() failed: %s\n", strerror(err));
			exit(EXIT_FAILURE);
		}
	}
	for (size_t  i= 0; i < NUM_THREADS; ++i){ 
		int s = pthread_join(tid[i], NULL);
	
		if (s != 0){
			printf("main: pthread_join for thread %ld failed\n", tid[i]);
			exit(EXIT_FAILURE);
		}	
		printf("main: thread %ld is joined succesfully\n", tid[i]);
		}
	free(tid);
 	
	return 0;
}

