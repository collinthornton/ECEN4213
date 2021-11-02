/*
Authors:	Max DeSantis, Collin Thornton
Exercise:	Lab 5 EX 1
*/

//Use g++ -std=c++11 -o Lab5EX1 Lab5EX1.cpp ../publisher.cpp -lwiringPi -pthread

#include <string>
#include <iostream>
#include <wiringSerial.h>
#include <wiringPi.h>
#include <unistd.h>
#include <cstdlib>
#include <iomanip>
#include "../publisher.hpp"
using namespace std;

int kobuki;

// Publisher handles input and sensor readings to the Kobuki
Publisher pub("/dev/kobuki", 115200);

int main(){

	// Start the publisher
	pub.run();

	// Every 20ms, print the publisher's sensor readings. If BTN 2 is high, exit.
	while(true) {
		printf("Bump: %d | Drop: %d | Cliff: %d | Btn: %d\n", pub.bumpSensor, pub.wheelSensor, pub.cliffSensor, pub.button);
		if(pub.button == Publisher::BTN1) {
			break;
		}
		usleep(20000);
	}

	return(0);
}
