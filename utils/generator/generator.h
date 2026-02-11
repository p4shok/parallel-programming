#pragma once

#include "./models/shared_memory.h"
#include "./logger/logger.h"

void generator_thread(int semid, SharedMemoryQueue* queue, Logger& logger);