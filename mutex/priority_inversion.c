#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

pthread_mutex_t mutex;

void* low_priority_thread(void* arg) {
    printf("[Low Priority] Waiting for mutex...\n");
    pthread_mutex_lock(&mutex);
    printf("[Low Priority] Acquired mutex.\n");
    sleep(3);
    printf("[Low Priority] Freeing mutex.\n");
    pthread_mutex_unlock(&mutex);
    return NULL;
}

void* medium_priority_thread(void* arg) {
    printf("[Medium Priority] Running independently...\n");
    sleep(2);
    return NULL;
}

void* high_priority_thread(void* arg) {
    printf("[High Priority] Waiting for mutex...\n");
    pthread_mutex_lock(&mutex);
    printf("[High Priority] Acquired mutex.\n");
    pthread_mutex_unlock(&mutex);
    return NULL;
}

int main() {
    pthread_t low_priority, medium_priority, high_priority;
    struct sched_param param;

    // Initialize mutex
    pthread_mutex_init(&mutex, NULL);

    // Create low priority thread
    pthread_create(&low_priority, NULL, low_priority_thread, NULL);
    param.sched_priority = 1;
    pthread_setschedparam(low_priority, SCHED_FIFO, &param);

    // Create medium priority thread
    pthread_create(&medium_priority, NULL, medium_priority_thread, NULL);
    param.sched_priority = 50;
    pthread_setschedparam(medium_priority, SCHED_FIFO, &param);

    // Create high priority thread
    pthread_create(&high_priority, NULL, high_priority_thread, NULL);
    param.sched_priority = 99;
    pthread_setschedparam(high_priority, SCHED_FIFO, &param);

    // Wait for threads to finish
    pthread_join(low_priority, NULL);
    pthread_join(medium_priority, NULL);
    pthread_join(high_priority, NULL);

    // Destroy mutex
    pthread_mutex_destroy(&mutex);

    return 0;
}
