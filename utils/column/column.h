#include "./models/shared_memory.h"
#include "./logger/logger.h"

void column_thread(int column_id, int fuel_type, int semid, SharedMemoryQueue* queue, Logger& logger);