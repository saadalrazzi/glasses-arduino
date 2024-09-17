#ifndef MOTOR_H
#define MOTOR_H

#include <Arduino.h>  // For analogWrite and millis()

class Motor {
public:
    Motor();
    Motor(int pin);         // Constructor
    void setAnalog(int val);  // Method to set analog value
    int getAnalog();         // Method to get analog value

private:
    int pin;   // Pin number
    int val;   // Last set value
};

#endif  // MOTOR_H