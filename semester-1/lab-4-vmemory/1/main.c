#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int global_init = 10;
int global_uninit;
const int global_const = 20;

int* return_local_addr() {
    int local_var = 42;
    return &local_var;
}

int main() {
    int local_main = 1;
    static int static_local = 2;
    const int const_local = 3;

    printf("PID: %d\n", getpid());
    printf("&local_main: %p\n", (void*)&local_main);
    printf("&static_local: %p\n", (void*)&static_local);
    printf("&const_local: %p\n", (void*)&const_local);
    printf("&global_init: %p\n", (void*)&global_init);
    printf("&global_uninit: %p\n", (void*)&global_uninit);
    printf("&global_const: %p\n", (void*)&global_const);

    int *bad_ptr = return_local_addr();
    printf("bad_ptr: %p\n", (void*)bad_ptr);

    char *buf1 = malloc(100);
    strcpy(buf1, "hello world");
    printf("buf1: %s\n", buf1);
    free(buf1);
    printf("buf1 after free: %s\n", buf1);

    char *buf2 = malloc(100);
    strcpy(buf2, "hello world");
    printf("buf2: %s\n", buf2);

    char *mid_ptr = buf2 + 50;
    // free(mid_ptr);

    char *my_env = getenv("MY_VAR");
    if (my_env) {
        printf("MY_VAR: %s\n", my_env);
        setenv("MY_VAR", "new_value", 1);
        printf("MY_VAR (new): %s\n", getenv("MY_VAR"));
    }

    sleep(100);
    return 0;
}