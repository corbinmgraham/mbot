#ifndef DATA_H
#define DATA_H

#include "Arduino.h"

class Data {
    private:
        String log;
    public:
        Data();
        ~Data() {}

        void log_data(String);
        String get_data();
};

#endif