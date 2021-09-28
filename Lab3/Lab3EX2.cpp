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

// functions
void sigroutine(int);
int adcVal();
float read_potentionmeter();
float read_sonar();


// variables
float distance_previous_error, distance_error;
float obj_value, measured_value; // potentionmeter reading, sonar reading
int adc;
int time_inter_ms = 23; // time interval, you can use different time interval

/*set your pin numbers and pid values*/
int motorPin = ;
int sonarPin = 1;

float kp= 11.4; 
float ki= 0; 
float kd= 0;


float distance, setpoint;
PID::PID pid(kp,ki,kd);

SONAR::SONAR sonar(sonarPin);

// Use adc.readmv()
ADC::ADC adc(0x48);


int main(){
	wiringPiSetup();
    sonar.run();
    adc.run();

    /*Set the pinMode (sonar and fan pins)*/

    
    // This part is to set a system timer, the function "sigroutine" will be triggered  
    // every time_inter_ms milliseconds. 
    struct itimerval value, ovalue;
    signal(SIGALRM, sigroutine);
    value.it_value.tv_sec = 0;
    value.it_value.tv_usec = time_inter_ms*1000;
    value.it_interval.tv_sec = 0;
    value.it_interval.tv_usec = time_inter_ms*1000;
    setitimer(ITIMER_REAL, &value, &ovalue);    

	while(true){
        cout<<"obj_value: "<<obj_value<<" measured_value: "<<measured_value<<endl;
        cout<<"PID_p: "<<pid.P<<endl;
        cout<<"PID_i: "<<pid.I<<endl;
        cout<<"PID_d: "<<pid.D<<endl;
        cout<<"PID_total: "<< pid.P + pid.I + pid.D << endl;
        delay(20);
	}
}


void sigroutine(int signo){
    pid.compute(distance, setpoint);
    return;
}



/* use a sonar sensor to measure the position of the Ping-Pang ball. you may reuse
your code in EX1.*/
void read_sonar()
{
    distance = sonar.read();
}

/* use a potentiometer to set an objective position (10 - 90 cm) of the Ping-Pang ball, varying the potentiometer
can change the objective distance. you may reuse your code in Lab 1.*/
void read_potentionmeter()
{
    // Probably put this logic in main loop rather than separate function. Store conversion elsewhere.
    double conversion = 80.0 / 3000.0;
    setpoint = 10 + adc.readmv()*conversion; //Never go below 10cm.
}

