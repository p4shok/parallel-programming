#include "generator.h"
#include "./models/semaphores.h"

#include <random>
#include <thread>
#include <chrono>

using namespace std;

void generator_thread(int semid, SharedMemoryQueue* queue, Logger& logger) {
    random_device rd;
    mt19937 gen(rd());
    normal_distribution<> gen_dist(MEAN_GENERATION_TIME, STD_GENERATION_TIME);
    uniform_int_distribution<> fuel_dist(0, 2);

    int request_id = 1;

    while (request_id <= TOTAL_REQUESTS) {
        double wait_time = gen_dist(gen);
        if (wait_time < 0) wait_time = 0;
        this_thread::sleep_for(chrono::milliseconds((int)wait_time));

        CarRequest request;
        request.id = request_id++;
        request.fuelType = fuel_dist(gen);

        logger.log_all_requests(request);

        semaphore_wait(semid, 1);
        semaphore_wait(semid, 0);

        if(!queue->is_full()) {
            queue->queue[queue->tail] = request;
            queue->tail = (queue->tail + 1) % MAX_QUEUE_SIZE;
            queue->count++;

            printf("[Генератор] Добавлена машина %d (тип %d). В очереди: %d\n",
                request.id, request.fuelType, queue->count);
        }

        semaphore_signal(semid, 0);
        semaphore_signal(semid, 2);
    }

    printf("[Генератор] Все %d заявок созданы. Завершение работы.\n", TOTAL_REQUESTS);
}