#include "Motor.h"

Motor::Motor() : pin(-1), val(0) {
    // Default pin is -1 (or any invalid value) and initial value is 0
}

// Constructor definition
Motor::Motor(int pin) : pin(pin), val(0) {
    pinMode(pin, OUTPUT);  // Initialize the pin as an output
}

// Method to set the analog value
void Motor::setAnalog(int val) {
    this->val = val;
    if(val==255){
    digitalWrite(this->pin, HIGH);  // Set the analog output
    }else{
    digitalWrite(this->pin, LOW);  // Set the analog output
      
    }
    // Serial.println("pin ");
    // Serial.println(this->pin);
}

// Method to get the analog value
int Motor::getAnalog() {
    return this->val;  // Return the last set value
}
