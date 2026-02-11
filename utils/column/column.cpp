#include "column.h"
#include "./models/semaphores.h"

#include <random>
#include <thread>
#include <chrono>

using namespace std;

void column_thread(int column_id, int fuel_type, int semid, SharedMemoryQueue* queue, Logger& logger) {
    random_device rd;
    mt19937 gen(rd());

    normal_distribution<> service_dist;
    if(fuel_type == 0) {
        service_dist = normal_distribution<>(MEAN_SERVICE_TIME_76, STD_SERVICE_TIME_76);
    } else if (fuel_type == 1) {
        service_dist = normal_distribution<>(MEAN_SERVICE_TIME_92, STD_SERVICE_TIME_92);
    } else {
        service_dist = normal_distribution<>(MEAN_SERVICE_TIME_95, STD_SERVICE_TIME_95);
    }
    
    while(true) {
        semaphore_wait(semid, 2);
        semaphore_wait(semid, 0);

        CarRequest request{-1, -1};

        if(!queue->is_empty()) {
            for (int i = 0; i < queue->count; i++) {
                int idx = (queue->head + i) % MAX_QUEUE_SIZE;
                if (queue->queue[idx].fuelType == fuel_type) {
                    request = queue->queue[idx];

                    for(int j = i; j < queue->count - 1; j++) {
                        int curr = (queue->head + j) % MAX_QUEUE_SIZE;
                        int next = (queue->head + j + 1) % MAX_QUEUE_SIZE;
                        queue->queue[curr] = queue->queue[next];
                    }

                    queue->count--;
                    queue->tail = (queue->tail - 1 + MAX_QUEUE_SIZE) % MAX_QUEUE_SIZE;

                    break;
                }
            }
        }

        semaphore_signal(semid, 0);

        if(request.id != -1) {
            semaphore_signal(semid, 1);

            printf("[Колонка %d] Обслуживаю машину %d (тип %d)\n", column_id, request.id, request.fuelType);

            double service_time = service_dist(gen);
            if (service_time < 0) service_time = 0;
            this_thread::sleep_for(chrono::milliseconds((int)service_time));

            logger.log_column_service(column_id, request);

            printf("[Колонка %d] Обслужила машину %d за %0.f мс.\n", column_id, request.id, service_time);
        } else {
            semaphore_signal(semid, 2);
        }
    }
}