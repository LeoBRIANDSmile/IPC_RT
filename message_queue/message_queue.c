#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mqueue.h>
#include <pthread.h>
#include <unistd.h>
#include <errno.h>

#define QUEUE_NAME "/example-queue"
#define MAX_MESSAGES 5
#define MAX_MSG_SIZE 256
#define MSG_BUFFER_SIZE MAX_MSG_SIZE
#define MAX_MSG_TO_SEND 10

void *client_task(void *arg) {

    mqd_t qd_client = mq_open(QUEUE_NAME, O_WRONLY | O_NONBLOCK);

    if (qd_client == (mqd_t) - 1) {
        perror("Client: mq_open");
        pthread_exit(NULL);
    }

    int i = 1;

    while (i < MAX_MSG_TO_SEND + 1) {
        
        static char in_buffer[MSG_BUFFER_SIZE];
        sprintf(in_buffer, "Message %d", i);

        if (mq_send(qd_client, in_buffer, strlen(in_buffer) + 1, 0) == -1) {
            if (errno == EAGAIN) {
                printf("[Client] Queue is full! Retrying...\n");
                sleep(1);
                continue;
            } else {
                perror("[Client] mq_send");
                break;
            }
        } else {
            printf("[Client] Sent: %s\n", in_buffer);
            i++;
        }
        usleep(200000);
    }

    mq_close(qd_client);
    pthread_exit(NULL);
}

void *server_task(void *arg) {

    mqd_t qd_server = mq_open(QUEUE_NAME, O_RDONLY);
    
    if (qd_server == (mqd_t) - 1) {
        perror("Server: mq_open");
        pthread_exit(NULL);
    }
    
    int i = 1;

    while (i < MAX_MSG_TO_SEND + 1) {

        static char out_buffer[MSG_BUFFER_SIZE];
        ssize_t bytes_read = mq_receive(qd_server, out_buffer, MSG_BUFFER_SIZE, NULL);
        if (bytes_read == -1) {
            perror("[Server] mq_receive");
        } else {
            printf("[Server] Received: %s\n", out_buffer);
            i++;
        }

        usleep(500000);
    }

    mq_close(qd_server);
    pthread_exit(NULL);
}

int main(void) {
    pthread_t client_thread, server_thread;
    struct mq_attr attr = {
        .mq_flags = O_NONBLOCK,
        .mq_maxmsg = MAX_MESSAGES,
        .mq_msgsize = MAX_MSG_SIZE,
        .mq_curmsgs = 0
    };

    mqd_t mq = mq_open(QUEUE_NAME, O_CREAT | O_RDWR, 0644, &attr);
    if (mq == (mqd_t)-1) {
        perror("Main: mq_open");
        exit(EXIT_FAILURE);
    }

    printf("Message Queue initialized with max messages: %ld, message size: %ld bytes\n",
           attr.mq_maxmsg, attr.mq_msgsize);
    
    printf("%d messages will be sent.\r\n\r\n", MAX_MSG_TO_SEND);

    // Create threads
    if (pthread_create(&client_thread, NULL, client_task, NULL) < 0) {
        perror("pthread_create");
        exit(EXIT_FAILURE);
    }

    if (pthread_create(&server_thread, NULL, server_task, NULL) < 0) {
        perror("pthread_create");
        exit(EXIT_FAILURE);
    }

    // Wait for threads to finish
    pthread_join(client_thread, NULL);
    pthread_join(server_thread, NULL);

    // Unlink message queue
    if (mq_unlink(QUEUE_NAME) == -1) {
        perror("Main: mq_unlink");
        exit(EXIT_FAILURE);
    }

    return 0;
}
