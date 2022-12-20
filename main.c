#include <pthread.h>
#include <stdio.h>

pthread_mutex_t mutex;

void* thread_func(void* arg) {
    // Lock the mutex before accessing the shared resource
    pthread_mutex_lock(&mutex);

    // Access the shared resource here

    // Unlock the mutex when finished accessing the shared resource
    pthread_mutex_unlock(&mutex);
    return NULL;
}

int main() {
    // Initialize the mutex
    pthread_mutex_init(&mutex, NULL);


    // Create the threads that will access the shared resource
    pthread_t thread1;
    pthread_create(&thread1, NULL, thread_func, NULL);

    // Wait for the threads to finish
    pthread_join(thread1, NULL);

    // Destroy the mutex when it is no longer needed
    pthread_mutex_destroy(&mutex);
    return 0;
}