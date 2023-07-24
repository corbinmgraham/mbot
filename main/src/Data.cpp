#include "Data.h"

Data::Data() {
    this->log = "";
}

/**
 * @brief Stores data to the log
 * 
 * @param data Any data that you want to save to log in string format
 */
void Data::log_data(String data) {
    this->log = log + data;
}

String Data::get_data() {
    return this->log;
}