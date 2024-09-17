#ifndef MOTORDRIVER_H
#define MOTORDRIVER_H

#include <string>
#include <Arduino.h>  // For analogWrite and millis()
#include "Motor.h"

class MotorDriver {
public:
    MotorDriver(int pins[], int num_motors);
    ~MotorDriver();  // Destructor to free allocated memory
    void setMsg(const std::string &msg);
    void executeMsg();
    void printMotorOutputs();
private:
    void parseMsg(float periods[]);

    unsigned long *next_off;
    unsigned long *next_set;

    Motor *motors;  // Pointer to dynamically allocate array of Motor objects
    std::string msg;
    int num_motors;  // Number of motors
};

#endif  // MOTORDRIVER_H
