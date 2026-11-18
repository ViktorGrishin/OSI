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
int global = 0;

void *mythread(void *arg) {
	printf("mythread [%d %d %d]: Hello from mythread!\n", getpid(), getppid(), gettid());
	int *local_ptr = (int *) arg; 
	int local = *local_ptr;

	static int ls = 15;
	const int lc = 72;
	printf("Thread %d: local = %d, global = %d\n", gettid(),  ++local, ++global);
	printf("\tlocal address = %p\n\tglobal_address = %p\n\tlocal_static = %p\n\tlocal const = %p\n\n", &local, &global, &ls, &lc);
	sleep(50);
	return NULL;
}

int main() {
 	global = 50;
	int err;
	pthread_t* tid = malloc(NUM_THREADS * sizeof(pthread_t));
	if (!tid){
		puts("main: malloc failure");
	} 
	
	int local = 3;	
	printf("Main [%d %d %d]: Hello from main!\n", getpid(), getppid(), gettid());
	printf("Main thread %d: local = %d, global = %d\n", gettid(),  ++local, ++global);
	for (int i = 0; i < NUM_THREADS; ++i){ 
		err = pthread_create(&tid[i], NULL, mythread, &local);
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

