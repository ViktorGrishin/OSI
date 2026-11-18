#define _GNU_SOURCE
#include <stdio.h>
#include <pthread.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <unistd.h>

void *mythread(void *arg) {
	printf("mythread [%d %d %d]: Hello from mythread!\n", getpid(), getppid(), gettid());
	int *ret = arg;
	*ret = 42;
	puts("Side stream is ended");
	return NULL;
}

int main() {
	pthread_t tid;
	int err;
	int result;
	printf("main [%d %d %d]: Hello from main!\n", getpid(), getppid(), gettid());

	err = pthread_create(&tid, NULL, mythread, &result);
	if (err) {
	    printf("main: pthread_create() failed: %s\n", strerror(err));
		return -1;
	}
	
	err = pthread_join(tid, NULL);
	if (err) {
	    puts("Error on join\n");
	    return -1;
	}
	printf("Result: %d\n", result);
	puts("Main ended");
	return 0;
}

