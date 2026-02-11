#include "semaphores.h"
#include "utils/conf/config.h"

#include <sys/sem.h>
#include <iostream>

int create_semaphore(key_t key, int num_sems) {
    int semid = semget(key, num_sems, IPC_CREAT | 0666);
    if (semid < 0) {
        perror("semid feild");
        exit(1);
    }

    union semun {
        int val;
        struct semid_ds *buf;
        unsigned short *array;
    } arg;

    unsigned short values[num_sems];
    values[0] = 1;
    values[1] = MAX_QUEUE_SIZE;
    values[2] = 0;

    arg.array = values;
    
    if (semctl(semid, 0, SETALL, arg) == -1) {
        perror("semctl SETALL failed");
        exit(1);
    }

    return semid;
}

void semaphore_wait(int semid, int sem_num) {
    struct sembuf op = {static_cast<unsigned short>(sem_num), -1, 0};
    if (semop(semid, &op, 1) == -1) {
        perror("semop wait failed");
        exit(1);
    }
}

void semaphore_signal(int semid, int sem_num) {
    struct sembuf op = {static_cast<unsigned short>(sem_num), 1, 0};
    if (semop(semid, &op, 1) == -1) {
        perror("semop signal failed");
        exit(1);
    }
}

void destroy_semaphore(int semid) {
    if(semctl(semid, 0, IPC_RMID) == -1) {
        perror("semctl IPC_RMID failed");
    }
}