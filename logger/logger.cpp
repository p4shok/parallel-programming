#include "logger.h"

#include <fstream>

using namespace std;

Logger::Logger() {
    column_files[0] = "all_requests.log";
    column_files[1] = "column_76_1.log";
    column_files[2] = "column_76_2.log";
    column_files[3] = "column_92_1.log";
    column_files[4] = "column_92_2.log";
    column_files[5] = "column_95.log";
}

void Logger::log_all_requests(const CarRequest& request) {
    lock_guard<mutex> lock(mtx_all);
    ofstream file(column_files[0], ios::app);
    file << "Заявка " << request.id << ", тип топлива: "
        << request.fuelType << endl;
}

void Logger::log_column_service(int column_id, const CarRequest& request) {
    if(column_id >= 1 && column_id <= 5) {
        lock_guard<mutex> lock(mtx_columns[column_id]);
        ofstream file(column_files[column_id], ios::app);
        file << "Обслужена машина " << request.id << ", тип топлива: " << request.fuelType << endl;
    }
}

void Logger::clear_logs() {
    for(int i = 0; i < 6; i++) {
        ofstream file(column_files[i], ios::trunc);
    }
}