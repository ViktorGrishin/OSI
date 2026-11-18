#define _GNU_SOURCE
#include <stdio.h>
#include <pthread.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <unistd.h>
#include <malloc.h>


void *mythread(void *arg) {
	int two = *(int *) arg;

	return (void *) (two + 1);
}


int main() {

	const int a = 15;
	pthread_t tid;
	int err;
	printf("main [%d %d %d]: Hello from main!\n", getpid(), getppid(), gettid());

	err = pthread_create(&tid, NULL, mythread, (void *) &a);

	if (err) {
	    printf("main: pthread_create() failed: %s\n", strerror(err));
		return -1;
	}
	int b;
	err = pthread_join(tid, (void *) &b);

	if (err) {
	    printf("main: pthread_join()) failed: %s\n", strerror(err));
		return -1;
	}

	printf("Main: returned value is %d\n", b);
	pthread_exit(0);
}

