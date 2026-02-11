#pragma once

#include <sys/ipc.h>

int create_semaphore(key_t key, int num_sems);
void semaphore_wait(int semid, int sem_num);
void semaphore_signal(int semid, int sem_num);
void destroy_semaphore(int semid);