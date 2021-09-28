#include "sonar.hpp"

#include <wiringPi.h>
#include <chrono>

using namespace std;
using namespace std::chrono;


SONAR::SONAR(short signalPin) {
    wiringPiSetup();
    pinMode(signalPin, OUTPUT);
    this->signalPin = signalPin;
}

void SONAR::run() {
    this->runThread = std::thread(&SONAR::runP, this);
}

void SONAR::stop() {
    this->stopThread = true;

    if(this->runThread.joinable())
        this->runThread.join();
}

double SONAR::read() {
    std::lock_guard<std::mutex> guard(this->m);
    double output = this->output;
    return output;
}

void SONAR::runP() {
	high_resolution_clock::time_point t1;
	high_resolution_clock::time_point t2;
	duration<double> pulse_width;

    short sigPin = this->signalPin;
    
    while(!this->stopThread) {
        pinMode(sigPin, OUTPUT);
        // Send pulse
        delayMicroseconds(2);
        digitalWrite(sigPin, HIGH);
        delayMicroseconds(5);
        digitalWrite(sigPin, LOW);

        pinMode(sigPin, INPUT);
        delayMicroseconds(750);
        
        /*Switch the pinMode to input and read the echo return pulse width*/
        
        t1 = high_resolution_clock::now();
        //t1 = micros();
        while(digitalRead(sigPin)) {
            t2 = high_resolution_clock::now();

            pulse_width = duration_cast<duration<double>>(t2 - t1);
            //printf("PUlse width: %f\n", pulse_width.count());
            if(pulse_width.count() >= this->tMax) break;
        }

        /*Calculate the distance.*/ //Speed of sound is 340m/s
        double distance = 340.0 * (float)pulse_width.count()*1E2 * 0.5;

 

        /*Delay before next measurement. The actual delay may be a little longer than what is shown is the datasheet.*/
        delayMicroseconds(2100);
        
        std::lock_guard<std::mutex> guard(this->m);
        this->output = smoothingFactor*distance + (1.0-smoothingFactor)*this->prevDistance;
        this->prevDistance = this->output;
    }
}