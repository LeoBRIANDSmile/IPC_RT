#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>
#include <time.h>

sem_t semaphore;

void* giver_task(void) {
    int i = 0;
    while (i < 10) {
        i++;
        sem_post(&semaphore);
        printf("[Giver] Semaphore given.\n");
        usleep(500000);
    }

    pthread_exit(NULL);
}

void* taker_task (void) {
    int i = 0;
    while (i < 10) {
        i++;
        sem_wait(&semaphore);
        printf("[Taker] Semaphore taken.\n");
    }

    pthread_exit(NULL);
}

int main (void)
{
    pthread_t giver_thread, taker_thread;

    // Initialize sempahore
    if (sem_init(&semaphore, 0, 0) != 0) {  // int sem_init(sem_t *sem, int pshared, unsigned int value);
        perror("sem_init");
        exit(EXIT_FAILURE);
    }

    // Create threads
    if (pthread_create (&giver_thread, NULL, (void*)giver_task, NULL) < 0) {
      perror ("pthread_create");
      exit (1);
    }

    if (pthread_create (&taker_thread, NULL, (void*)taker_task, NULL) < 0) {
      perror ("pthread_create");
      exit (1);
    }

    // Wait for threads to finish
    pthread_join(giver_thread, NULL);
    pthread_join(taker_thread, NULL);

    // Détruire le sémaphore
    sem_destroy(&semaphore);


}
