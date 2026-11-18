#define _GNU_SOURCE
#include <stdio.h>
#include <pthread.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>

void *mythread(void *arg) {
	printf("mythread [%d %d %d]: Hello from mythread!\n", getpid(), getppid(), gettid());
	return NULL;
}

int main() {
	pthread_t tid;
	int err;

	printf("main [%d %d %d]: Hello from main!\n", getpid(), getppid(), gettid());

	err = pthread_create(&tid, NULL, mythread, NULL);
	if (err) {
	    printf("main: pthread_create() failed: %s\n", strerror(err));
		return -1;
	}
	
	int s = pthread_join(tid, NULL);
	
	if (s != 0){
		printf("main: pthread_join failed\n");
		exit(EXIT_FAILURE);
}
	puts("main: thread is joined succesfully"); 	
	return 0;
}

