#define _GNU_SOURCE
#include <stdio.h>
#include <pthread.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <unistd.h>
#include <malloc.h>

typedef struct {
    int a;
	char* string;
} myStruct;

void *mythread(void *arg) {
	myStruct* two = (myStruct *) arg;
	printf("mythread [%d %d %d]: Hello from mythread!\n", getpid(), getppid(), gettid());
	printf("Arguments: \n\ta = %d\n\tString = '%s'\n", two->a, two->string);
	free(two);
	return NULL;
}


int main() {
	pthread_t tid;
	int err;
	printf("main [%d %d %d]: Hello from main!\n", getpid(), getppid(), gettid());



	myStruct* one = (myStruct *) malloc(sizeof(myStruct));
	one -> a = 7, one -> string = "Hello world!";
	

	pthread_attr_t attr;
	pthread_attr_init(&attr);
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
	err = pthread_create(&tid, NULL, mythread, (void *) one);
	pthread_attr_destroy(&attr);
	if (err) {
	    printf("main: pthread_create() failed: %s\n", strerror(err));
		return -1;
	}
	
	return 0;
}

