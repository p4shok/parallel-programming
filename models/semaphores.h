#pragma once

#include <sys/ipc.h>

int create_semaphores(key_t key, int num_sems);
void semaphore_wait(int semid, int sem_num);
void semaphore_signal(int semid, int sem_num);
void destroy_semaphores(int semid);