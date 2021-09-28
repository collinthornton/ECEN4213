#include "PID.hpp"
#include <wiringPi.h>

PID::PID(double Kp, double Ki, double Kd) {
    this->Kp = Kp;
    this->Ki = Ki;
    this->Kd = Kd;
    this->integral = 0;
    this->prevError = 0;
    this->prevTime = 0;
}

int PID::compute(double input, double setpoint) {
    double error = input - setpoint;
    double time = (double)micros()*1.0e-6;
    
    this->P =  this->Kp*error;
    this->I += this->Ki*error*(time-prevTime);
    this->D =  this->Kd*(error-this->prevError)/(time - this->prevTime);
    
    this->prevError = error;
    this->prevTime = time;


    int cntrl = this->P + this->I + this->D;
    return cntrl;
}

void PID::reset(void) {
    this->prevTime = micros();
    this->prevError = 0;
    this->integral = 0;
}