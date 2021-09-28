/*
Authors:	Max DeSantis, Collin Thornton
Exercise:	Lab 3 EX1
Note:		We built our own class, called SONAR, to handle the sensor readings.
			It runs in a separate thread (started by sonar.run()) and handles all timings.
			calling sonar.read() provides the latest value of a running average measurement, improving accuracy.
*/

//Use g++ -std=c++11 -o Lab3EX1 Lab3EX1.cpp sonar.cpp -lwiringPi -lpthread

#include <iostream>
#include <unistd.h>
#include <stdio.h>
#include <wiringPi.h>
#include <ctime>
#include <ratio>
#include <chrono>
#include <thread>
#include "sonar.hpp"
using namespace std;
using namespace std::chrono;

int sigPin = 1;

// Init sonar object (sets up WiringPi inside)
SONAR sonar(sigPin);


int main(){
	// Start sonar thread
	sonar.run();
	
	// Every 10ms, read and print the distance reading.
	while(true) {
		printf("Distance: %6.3f\tAlpha: %6.3f\tPrev: %6.3f\n", sonar.read(), sonar.smoothingFactor, sonar.prevDistance);
		delay(10);
	}

}
