/*
Authors:    Max DeSantis, Collin Thornton
Exercise:   Lab 3 EX 2
*/

//Use g++ -std=c++11 -o Lab3EX2 Lab3EX2.cpp sonar.cpp PID.cpp adc.cpp -lwiringPi -lpthread

#include <iostream>
#include <iomanip>
#include <unistd.h>
#include <wiringPi.h>
#include <wiringPiI2C.h>
#include <signal.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <sys/time.h>
#include <chrono>
#include <cmath>

#include "PID.hpp"
#include "sonar.hpp"
#include "adc.hpp"

using namespace std::chrono;
using namespace std;


/*set your pin numbers and pid values*/
int motorPin = 23;
short sonarPin = 1;

float kp= 6; 
float ki= 4; 
float kd= 3;

float height = 102.0;
float measuredDistance = 0;
float setpoint = 0;
int pwmVal;

double loopRate = 10;

PID pid{kp,ki,kd, loopRate*1e-3};
SONAR sonar{sonarPin};

// Use adc.readmv()
ADC adc{0x48};


int main(){
	wiringPiSetup();
    sonar.run();
    adc.run();
    pinMode(motorPin, PWM_OUTPUT);
    int a = 0;
    printf("Getting started\n");
    pwmWrite(motorPin, 0);

	while(true){

        measuredDistance = sonar.read();
        setpoint = 10.0 + adc.readmv() * (80.0/5100.0);
        
        pwmVal = pid.compute(measuredDistance, setpoint) + 920;
        //pwmWrite(motorPin, min(1023, max(pwmVal, 0)));
        pwmWrite(motorPin, pwmVal);
        printf("Setpoint: %7.3f | Measured: %7.3f | PWM: %5d | loop %5d | P %5.3f | I %5.3f | D %5.3f\n", setpoint, measuredDistance, pwmVal, a, pid.P, pid.I, pid.D);
        a++;
        delay(loopRate);
	}
}