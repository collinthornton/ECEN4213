#include "PID.hpp"
#include <wiringPi.h>

PID::PID(double Kp, double Ki, double Kd, double minPeriod) {
    // Initialize gains and other class variables

    this->Kp = Kp;
    this->Ki = Ki;
    this->Kd = Kd;
    this->integral = 0;
    this->prevError = 0;
    this->prevTime = 0;
    this->minPeriod = minPeriod;
}

int PID::compute(double input, double setpoint) {
    // Compute the output

    double error = input - setpoint;
    double time = (double)micros()*1.0e-6;
    double dt = time - prevTime;

    // Compute P, I, & D control terms
    this->P =  this->Kp*error;
    this->I += this->Ki*error*dt;
    this->D =  dt >= this->minPeriod ? this->Kd*(error-this->prevError)/dt : this->D;
    
    // Remember the error
    this->prevError = error;
    this->prevTime = time;

    // Ouput = sum of control terms
    int cntrl = this->P + this->I + this->D;
    return cntrl;
}

void PID::reset(void) {
    // Reset integral of the error

    this->prevTime = micros();
    this->prevError = 0;
    this->integral = 0;
}