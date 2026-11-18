#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <signal.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <unistd.h>

static void sigint_handler(int sig) {
    printf("Thread 2 [tid=%ld]: find SIGINT (%d)\n",
           (long)gettid(), sig);
}

static void *thread_block_all(void *arg) {
    sigset_t all;
    sigfillset(&all);
    int err = pthread_sigmask(SIG_BLOCK, &all, NULL);
    if (err) {
        fprintf(stderr, "thread 1: pthread_sigmask failed: %s\n", strerror(err));
		exit(EXIT_FAILURE);
	}
    printf("Thread 1 [tid=%ld]: blocked all signals\n", (long)gettid());

    sleep(30);
    return NULL;
}

static void *thread_sigint_handler(void *arg) {
    sigset_t set;
    sigemptyset(&set);
    sigaddset(&set, SIGINT);
    int err = pthread_sigmask(SIG_UNBLOCK, &set, NULL);
    if (err) {
        fprintf(stderr, "thread 2: pthread_sigmask failed: %s\n", strerror(err));
        exit(EXIT_FAILURE);
    }

    struct sigaction sa;
    memset(&sa, 0, sizeof(sa));
    sa.sa_handler = sigint_handler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    if (sigaction(SIGINT, &sa, NULL) == -1) {
        perror("thread 2: sigaction");
        exit(EXIT_FAILURE);
    }

    printf("Thread 2 [tid=%ld]: waiting for SIGINT\n", (long)gettid());

    pause();

    printf("Thread 2 [tid=%ld]: start after pause\n", (long)gettid());
    return NULL;
}

static void *thread_sigwait_quit(void *arg) {
    sigset_t set;
    sigemptyset(&set);
    sigaddset(&set, SIGQUIT);
    printf("Thread 3 [tid=%ld]: sigwait() for SIGQUIT\n", (long)gettid());

    int sig;
    int err = sigwait(&set, &sig);
    if (err) {
        fprintf(stderr, "thread 3: sigwait failed: %s\n", strerror(err));
    } else {
        printf("Thread 3 [tid=%ld]: sigwait() return signal %d\n",
               (long)gettid(), sig);
    }
    return NULL;
}

int main(void) {
    printf("main [pid=%d, tid=%ld]\n",
           getpid(), (long)gettid());

    sigset_t blocked;
    sigemptyset(&blocked);
    sigaddset(&blocked, SIGINT);
    sigaddset(&blocked, SIGQUIT);

    int err = pthread_sigmask(SIG_BLOCK, &blocked, NULL);
    if (err) {
        fprintf(stderr, "main: pthread_sigmask failed: %s\n", strerror(err));
        exit(EXIT_FAILURE);
    }
    printf("main: SIGINT and SIGQUIT are blocked\n");

    pthread_t t1, t2, t3;

    err = pthread_create(&t1, NULL, thread_block_all, NULL);
    if (err) {
        fprintf(stderr, "main: pthread_create (t1) failed: %s\n", strerror(err));
        return EXIT_FAILURE;
    }

    err = pthread_create(&t2, NULL, thread_sigint_handler, NULL);
    if (err) {
        fprintf(stderr, "main: pthread_create (t2) failed: %s\n", strerror(err));
        return EXIT_FAILURE;
    }

    err = pthread_create(&t3, NULL, thread_sigwait_quit, NULL);
    if (err) {
        fprintf(stderr, "main: pthread_create (t3) failed: %s\n", strerror(err));
        return EXIT_FAILURE;
    }

    pthread_join(t1, NULL);
    pthread_join(t2, NULL);
    pthread_join(t3, NULL);

    printf("main: all threads finished\n");
    return EXIT_SUCCESS;
}