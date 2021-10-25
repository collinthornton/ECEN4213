#include "sonar.hpp"

#include <wiringPi.h>
#include <chrono>
#include <unistd.h>

using namespace std;
using namespace std::chrono;


SONAR::SONAR(short signalPin) {
    // Setup wiringPi. Initialize signal pin as input.
    wiringPiSetup();
    pinMode(signalPin, OUTPUT);
    this->signalPin = signalPin;
}

void SONAR::run() {
    // Public function to start thread
    this->runThread = std::thread(&SONAR::runP, this);
}

void SONAR::stop() {
    // Public function to kill thread
    this->stopThread = true;

    if(this->runThread.joinable())
        this->runThread.join();
}

double SONAR::read() {
    // Public function to poll current value

    // Data protected with mutex
    std::lock_guard<std::mutex> guard(this->m);
    double output = this->output;
    return output;
}

void SONAR::runP() {
    // Private function to continually poll the sonar sensor

    // Use high resolution clocks
	high_resolution_clock::time_point t1;
	high_resolution_clock::time_point t2;
    int pulse_width;

    // Make a short name for the signal pin
    short sigPin = this->signalPin;

    // Run continuously until stopped
    while(!this->stopThread) {
        pinMode(sigPin, OUTPUT);
        // Send pulse
        digitalWrite(sigPin, LOW);
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

            pulse_width = duration_cast<chrono::microseconds>(t2 - t1).count();
            //printf("PUlse width: %f\n", pulse_width.count());
            if(pulse_width >= this->tMax) break;
        }
        if(pulse_width >= this->tMax) continue;
        /*Calculate the distance.*/ //Speed of sound is 340m/s
        double distance = (17 * pulse_width) / 1000;
 

        // Protect data with a mutex and update current estimate
        {
        std::lock_guard<std::mutex> guard(this->m);
        this->output = smoothingFactor*distance + (1.0-smoothingFactor)*this->prevDistance;
        this->prevDistance = this->output;
        }
        /*Delay before next measurement. The actual delay may be a little longer than what is shown is the datasheet.*/
        delayMicroseconds(20000);
    }
}