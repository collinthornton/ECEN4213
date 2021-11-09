/*
Authors:	Max DeSantis, Collin Thornton
Exercise:	Lab 5 EX2
Note:		We used the SONAR class from Lab 3, which handles sonar measurements in a separate thread. We also created the Publisher class,
			which handles all information transfer between the Pi and Kobuki.
*/

//use g++ -std=c++11 -o Lab5EX2 Lab5EX2.cpp ../publisher.cpp ../sonar.cpp -lwiringPi -pthread


#include <iostream>
#include <unistd.h>
#include <stdio.h>
#include <wiringPi.h>
#include <wiringSerial.h>
#include <ctime>
#include <ratio>
#include "../publisher.hpp"
#include "../sonar.hpp"
#include <chrono>
using namespace std;
using namespace std::chrono;

int sonarPin = 1;

// Init other threads
Publisher pub("/dev/kobuki", 115200);
SONAR sonar(sonarPin);

int main(){
	// Start other threads
	sonar.run();
	pub.run();

	// Ignore initial -20 reading from sonar
	delay(500);
	
	// Move forward
	pub.move(400, 0);

	// Print debug info
	printf("Sonar: %f | Wheel: %d | Cliff: %d | Bumper: %d | Btn: %d\n", sonar.read(), pub.wheelSensor, pub.cliffSensor, pub.bumperSensor, pub.button);
	
	// Continue moving until the sonar is less than 25cm from the wall
	while(sonar.read() >= 25) {
		printf("Sonar: %f | Wheel: %d | Cliff: %d | Bumper: %d | Btn: %d\n", sonar.read(), pub.wheelSensor, pub.cliffSensor, pub.bumperSensor, pub.button);
		delay(10);
	}
	
	// Stop moving and wait a moment before turning
	pub.move(0, 0);
	delay(200);
	
	// Turn 90 degrees clockwise (pi/2 wasn't entirely accurate)
	pub.rotate(3.145 / 2.35, -3.145 / 6);
	pub.move(0, 0);
	delay(200);

	// Start moving. Continue until less than 25cm from wall.
	pub.move(400, 0);
	while(sonar.read() >= 25) {
		printf("Sonar: %f | Wheel: %d | Cliff: %d | Bumper: %d | Btn: %d\n", sonar.read(), pub.wheelSensor, pub.cliffSensor, pub.bumperSensor, pub.button);

		delay(10);
	}
	
	// Stop and wait
	pub.move(0, 0);
	delay(200);
	
	// Turn 90 degrees counter clock wise
	pub.rotate(3.145 / 2.35, 3.145 / 6);
	pub.move(0, 0);
	delay(200);

	// Move final 2.5m
	pub.moveDist(2500, 400, 0);

	delay(1000);
}
