#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/shm.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <pthread.h>
#include <semaphore.h>

#define SHM_NAME "/shared_object"
#define SHM_SEM_NAME "/shm_sem"
#define MAX_MESSAGE_SIZE 256
#define NUM_CONSUMERS 5

// SHM struct 
typedef struct {
    char message[MAX_MESSAGE_SIZE];
    int available; 
} SharedData;

SharedData *shm_ptr;
sem_t *shm_sem;

// Init functions

void init_shm_and_semaphore() {
    // Create binary semaphore for shm synchro
    shm_sem = sem_open(SHM_SEM_NAME, O_CREAT, 0644, 1);
    if (shm_sem == SEM_FAILED) {
        perror("sem_open");
        exit(EXIT_FAILURE);
    }

    // Create the shared memory object
    int shm_fd;
    if ((shm_fd = shm_open(SHM_NAME, O_CREAT | O_RDWR, 0666)) == -1) {
        perror ("shm_open");
        exit(EXIT_FAILURE);
    }

    // Define shm size
    if (ftruncate(shm_fd, sizeof(SharedData)) == -1) {
        perror("ftruncate");
        exit(EXIT_FAILURE);
    }

    // Map shm
    shm_ptr = (SharedData *)mmap(NULL, sizeof(SharedData), PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);
    if (shm_ptr == MAP_FAILED) {
        perror("mmap");
        exit(EXIT_FAILURE);
    }

    // Initialize shm with 0s
    memset(shm_ptr, 0, sizeof(SharedData));
}

// RT Tasks
void* producer_task(void) {

    int message_id = 1;
    
    while (message_id <= 10) {
        sem_wait(shm_sem);

        snprintf(shm_ptr->message, MAX_MESSAGE_SIZE, "Message %d", message_id);
        shm_ptr->available = 1;

        printf("[Producer] Message %d sent : %s\n", message_id, shm_ptr->message);
        
        message_id++;

        sem_post(shm_sem);

        sleep(1);
    }

    pthread_exit(NULL);
}


void* consumer_task(void* arg) {

    int consumer_id = *((int *)arg);

    while (1) {
        sem_wait(shm_sem);

        if (shm_ptr->available) {
            printf("[Consumer %d] Message received : %s\n", consumer_id, shm_ptr->message);
            shm_ptr->available = 0;
        }

        sem_post(shm_sem);

        sleep(1);
    }

    pthread_exit(NULL);
}

int main (void) {

    init_shm_and_semaphore();

    pthread_t producer_thread, consumer_thread;
    pthread_t consumer_threads[NUM_CONSUMERS];
    int consumer_ids[NUM_CONSUMERS];

    // Create threads
    for (int i = 0; i < NUM_CONSUMERS; i++) {
        consumer_ids[i] = i + 1; 
        if (pthread_create(&consumer_threads[i], NULL, consumer_task, &consumer_ids[i]) < 0) {
            perror("pthread_create");
            exit(EXIT_FAILURE);
        }
    }

    if (pthread_create (&producer_thread, NULL, (void*)producer_task, NULL) < 0) {
        perror ("pthread_create");
        exit(EXIT_FAILURE);
    }

    // Wait for threads to finish
    pthread_join(producer_thread, NULL);
    for (int i = 0; i < NUM_CONSUMERS; i++) {
        pthread_join(consumer_threads[i], NULL);
    }

    // Nettoyage
    sem_close(shm_sem);
    sem_unlink(SHM_SEM_NAME);
    shm_unlink(SHM_NAME);


    return EXIT_SUCCESS;
}