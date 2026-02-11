#include "shared_memory.h"

#include <iostream>

int create_shm(key_t key, size_t size) {
    int shmid = shmget(key, size, 0666 | IPC_CREAT);

    if (shmid < 0) {
        perror("shmid failed");
        exit(1);
    }

    return shmid;
}

SharedMemoryQueue* attach_shm(int shmid) {
    void* ptr = shmat(shmid, NULL, 0);

    if(ptr == (void*)-1) {
        perror("shmat failed");
        exit(1);
    }

    return (SharedMemoryQueue*) ptr;
}

void detach_shm(void* ptr) {
    if(shmdt(ptr) == -1) {
        perror("shmdt failed");
    }
}

void destroy_shm(int shmid) {
    if(shmctl(shmid, IPC_RMID, NULL) == -1) {
        perror("shmctl IPC_RMID failed");
    }
}