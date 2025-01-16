#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#define NUM_THREADS 5
#define NUM_ITERATIONS 3

int shared_resource = 0;
pthread_mutex_t mutex;

// Function executed by each thread
void* thread_task(void* arg) {
    int thread_id = *(int*)arg;

    for (int i = 0; i < NUM_ITERATIONS; i++) {
        // Lock the mutex before accessing the shared resource
        pthread_mutex_lock(&mutex);

        printf("\r\n[Thread %d] Accessing shared resource...\n", thread_id);
        shared_resource++;
        printf("[Thread %d] Shared resource after modification: %d\n", thread_id, shared_resource);

        sleep(1);

        pthread_mutex_unlock(&mutex);

        usleep(100000);
    }

    pthread_exit(NULL);
}


int main() {
    pthread_t threads[NUM_THREADS];
    int thread_ids[NUM_THREADS];

    // Initialize the mutex
    if (pthread_mutex_init(&mutex, NULL) != 0) {
        perror("Error initializing the mutex");
        return EXIT_FAILURE;
    }

    // Create the threads
    for (int i = 0; i < NUM_THREADS; i++) {
        thread_ids[i] = i + 1;
        if (pthread_create(&threads[i], NULL, thread_task, &thread_ids[i]) != 0) {
            perror("Error creating the thread");
            return EXIT_FAILURE;
        }
    }

    // Wait for all threads to finish
    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }

    // Destroy the mutex
    pthread_mutex_destroy(&mutex);

    printf("Program finished. Final shared resource value: %d\n", shared_resource);
    return EXIT_SUCCESS;
}
