#include <time.h>
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <fcntl.h>
#include <signal.h>
#include <stdlib.h>
#include <sys/wait.h>

#define NUM 1000
int shared_resource = 0;
pthread_cond_t cond;
long Endtime;
long Starttime;
pthread_mutex_t mutex;

long ReturnTimeNs() {
    struct timespec currTime;

    if (clock_gettime(CLOCK_REALTIME, &currTime) == -1) {
        perror("clock gettime");
        exit(1);
    }
    return currTime.tv_nsec;
}

void *thread_func(void *arg) {

    // Lock the mutex before accessing the shared resource
    pthread_mutex_lock(&mutex);

    // Access the shared resource here

    // Unlock the mutex when finished accessing the shared resource
    pthread_mutex_unlock(&mutex);

    return NULL;
}

void mutex1() {
    int i;

    // Initialize the mutex
    pthread_mutex_init(&mutex, NULL);
    Starttime = ReturnTimeNs();
    for (i = 0; i < NUM; i++) {
        // Create the threads that will access the shared resource
        pthread_t thread1;
        pthread_create(&thread1, NULL, thread_func, NULL);

        // Wait for the threads to finish
        pthread_join(thread1, NULL);
    }
    Endtime = ReturnTimeNs();
    printf("The time for mutex is %ld\n", (Endtime - Starttime));
    // Destroy the mutex when it is no longer needed
    pthread_mutex_destroy(&mutex);

}

void fcntl1() {
    Starttime = ReturnTimeNs();
    if (fork() == 0) {
        for (int i = 0; i < NUM; i++) {
            // Open the file for reading and writing
            int fd = open("file.txt", O_RDWR);
            if (fd < 0) {
                perror("error in open");

            }
            // Set the lock type to a shared lock
            struct flock lock;
            lock.l_type = F_RDLCK;
            lock.l_whence = SEEK_SET;
            lock.l_start = 0;
            lock.l_len = 0; // Lock the entire file

            // Acquire the lock
            if (fcntl(fd, F_SETLKW, &lock) < 0) {
                perror("fcntl");
            }

            // Release the lock
            lock.l_type = F_UNLCK;
            if (fcntl(fd, F_SETLK, &lock) < 0) {
                perror("fcntl");
            }
            close(fd);
        }
    } else {
        wait(NULL);
        Endtime = ReturnTimeNs();
        printf("The time for fcntl is %ld\n", (Endtime - Starttime));
    }

}

void *thread_func_cond(void *arg) {
    pthread_mutex_lock(&mutex);

    // Wait for the shared resource to become available
    while (shared_resource == 0) {
        pthread_cond_wait(&cond, &mutex);
    }

    // Access the shared resource here

    // Unlock the mutex
    pthread_mutex_unlock(&mutex);
    return NULL;
}

void cond1() {
    Starttime = ReturnTimeNs();
    for(int i =0 ; i<NUM; i++) {
        pthread_t thread1;
        // Initialize the mutex and condition variable
        pthread_mutex_init(&mutex, NULL);
        pthread_cond_init(&cond, NULL);
        // Create the threads that will access the shared resource
        pthread_create(&thread1, NULL, thread_func_cond, NULL);
        pthread_mutex_lock(&mutex);
        shared_resource = 1;
        pthread_cond_signal(&cond);
        pthread_mutex_unlock(&mutex);

        // Wait for the threads to finish
        pthread_join(thread1, NULL);
        //printf("daniel is tired ");
        // Destroy the mutex and condition variable when they are no longer needed
        pthread_mutex_destroy(&mutex);
        pthread_cond_destroy(&cond);
    }
    Endtime = ReturnTimeNs();
    printf("The cond time is %ld\n",(Endtime-Starttime));
}


int main() {

    mutex1();
    cond1();
    fcntl1();
    return 0;
}