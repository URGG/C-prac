#include <stdio.h>
#include <pthread.h>


void* foo(void* arg) {
    printf("created a new thread");
    return NULL;
}

int main() {
    //crate a new thrad variable
    pthread_t thread1;

    //creating a new thread
    pthread_create(&thread1, NULL, foo, NULL);
    return 0;
}

