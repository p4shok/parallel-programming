#pragma once

#include "car_request.h"
#include "./utils/conf/config.h"

#include <sys/ipc.h>
#include <sys/shm.h>

struct SharedMemoryQueue {
    CarRequest queue[MAX_QUEUE_SIZE];
    int head;
    int tail;
    int count;

    bool is_empty() {
        return count == 0;
    }

    bool is_full() {
        return count == MAX_QUEUE_SIZE;
    }
};

int create_shm(key_t key, size_t size);
SharedMemoryQueue* attach_shm(int shmid);
void detach_shm(void* ptr);
void destroy_shm(int shmid);