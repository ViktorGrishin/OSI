#define _GNU_SOURCE
#include <stdio.h>
#include <pthread.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <unistd.h>


typedef struct {
    int a;
	char* string;
} myStruct;

void *mythread(void *arg) {
	myStruct* two = (myStruct *) arg;
	printf("mythread [%d %d %d]: Hello from mythread!\n", getpid(), getppid(), gettid());
	printf("Arguments: \n\ta = %d\n\tString = '%s'\n", two->a, two->string);
	return NULL;
}


int main() {
	pthread_t tid;
	int err;

	myStruct one = {.a = 7, .string = "Hello world!"};
	

	printf("main [%d %d %d]: Hello from main!\n", getpid(), getppid(), gettid());

	err = pthread_create(&tid, NULL, mythread, (void *) &one);
	if (err) {
	    printf("main: pthread_create() failed: %s\n", strerror(err));
		return -1;
	}

	int s = pthread_join(tid, NULL);
	
	if (s != 0){
		printf("main: pthread_join failed\n");
		return -1;
	}
	
	return 0;
}

