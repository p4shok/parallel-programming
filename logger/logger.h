#pragma once

#include "models/car_request.h"

#include <string>
#include <mutex>

using namespace std;

class Logger {
    private:
        mutex mtx_all;
        mutex mtx_columns[6];
        string column_files[6];

    public:
        Logger();

        void log_all_requests(const CarRequest& request);
        void log_column_service(int column_id, const CarRequest& request);
        void clear_logs();
};