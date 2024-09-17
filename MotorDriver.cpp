#include "MotorDriver.h"
#include <sstream>
#include <cmath>  // For round function

// Constructor definition
MotorDriver::MotorDriver(int pins[], int num_motors) : num_motors(num_motors) {
    this->next_off = new unsigned long[num_motors];
    this->next_set = new unsigned long[num_motors];
    this->motors = new Motor[num_motors];  // Allocate memory for Motor objects

    for (int i = 0; i < num_motors; ++i) {
        this->next_off[i] = 0;
        this->next_set[i] = 0;
        this->motors[i] = Motor(pins[i]);  // Initialize each Motor object with the corresponding pin
    }
}

// Destructor definition
MotorDriver::~MotorDriver() {
    delete[] next_off;     // Free allocated memory
    delete[] next_set;
    delete[] motors;       // Free Motor array
}

// Method to set the message
void MotorDriver::setMsg(const std::string &msg) {
    this->msg = msg;  // Correctly assign the string
}

// Method to execute message
void MotorDriver::executeMsg() {
    unsigned long currentMillis = millis();  // Get current time

    if (msg != "No New Data") {
        float *periods = new float[num_motors];  // Declare array of periods dynamically
        parseMsg(periods);  // Parse message into periods

        // Update periods
        for (int i = 0; i < num_motors; i++) {
            if (next_set[i] < currentMillis) {
                if (periods[i] > 0) {
                    motors[i].setAnalog(255);  // Write analog value (0-255)
                    next_set[i] = currentMillis + periods[i];
                    next_off[i] = round(currentMillis + periods[i] / 2.0);
                }
            }
        }

        delete[] periods;  // Free allocated memory
        this->msg = "No New Data";  // Set message to indicate no new data
    }

    // Set motors off if necessary
    for (int i = 0; i < num_motors; i++) {
        if (next_off[i] < currentMillis) {
            motors[i].setAnalog(0);  // Turn off the motor
        }
    }
}

// Method to parse message
void MotorDriver::parseMsg(float periods[]) {
    std::stringstream ss(this->msg);
    std::string token;
    int i = 0;

    while (std::getline(ss, token, ';') && i < num_motors) {
        periods[i++] = std::stof(token);  // Convert each token to float and store in periods array
    }
}

// Method to print motor outputs
void MotorDriver::printMotorOutputs() {
    for (int i = 0; i < num_motors; i++) {
        Serial.print(motors[i].getAnalog());  // Print each motor's output
        if (i < num_motors - 1) {
            Serial.print("| ");
        }
    }
    Serial.println();
}
