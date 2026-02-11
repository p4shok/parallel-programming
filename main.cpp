#include "./models/shared_memory.h"
#include "./models/semaphores.h"
#include "./logger/logger.h"
#include "./utils/generator/generator.h"
#include "./utils/column/column.h"

#include <iostream>
#include <signal.h>
#include <vector>
#include <thread>

using namespace std;

int shmid = -1;
int semid = -1;
SharedMemoryQueue* queue = nullptr;

void cleanup(int signum) {
    cout << "Получен сигнал завершения. Очистка ресурсов..." << endl;

    if(queue) {
        detach_shm(queue);
    }
    
    if (shmid != -1) {
        destroy_shm(shmid);
    }

    if(semid != -1) {
        destroy_semaphores(semid);
    }

    exit(0);
}

int main() {
    signal(SIGINT, cleanup);
    signal(SIGTERM, cleanup);

    Logger logger;
    logger.clear_logs();

    key_t shm_key = ftok(".", 'S');
    key_t sem_key = ftok(".", 'M');

    if(shm_key == -1 || sem_key == -1) {
        perror("ftok failed");
        return 1;
    }

    shmid = create_shm(shm_key, sizeof(SharedMemoryQueue));
    queue = attach_shm(shmid);

    queue->head = 0;
    queue->tail = 0;
    queue->count = 0;

    semid = create_semaphores(sem_key, 3);

    vector<thread> threads;

    threads.push_back(thread(generator_thread, semid, queue, ref(logger)));

    threads.push_back(thread(column_thread, 1, 0, semid, queue, ref(logger)));
    threads.push_back(thread(column_thread, 2, 0, semid, queue, ref(logger)));

    threads.push_back(thread(column_thread, 3, 1, semid, queue, ref(logger)));
    threads.push_back(thread(column_thread, 4, 1, semid, queue, ref(logger)));

    threads.push_back(thread(column_thread, 5, 2, semid, queue, ref(logger)));

    threads[0].join();

    cout << "Генератор завершил работу. Ожидание обработки оставшихся заявок..." << endl;
    this_thread::sleep_for(chrono::seconds(5));

    cleanup(0);

    return 0;
}