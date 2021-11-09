/*
Authors:	Max DeSantis, Collin Thornton
Exercise:	Lab 5 Ex 3
Note:		We created the publisher class to handle comms between the Kobuki and Pi. It runs a separate thread.
*/

//Use g++ -std=c++11 -o Lab5EX3 Lab5EX3.cpp ../publisher.cpp -pthread -lwiringPi

#include <string>
#include <iostream>
#include <wiringSerial.h>
#include <wiringPi.h>
#include <unistd.h>
#include <cstdlib>
#include <iomanip>
#include "../publisher.hpp"
using namespace std;


// Declare publisher class
Publisher pub("/dev/kobuki", 115200);

int main(){
	// Start publisher thread
	pub.run();
	
	// Continuously move forward. If a sensor is tripped (wheel drop, bumper, cliff) then stop, reverse 15cm, and rotate clockwise a small amount. Then repeat.
	while(true) {
		// Exit if button is pressed.
		if(pub.button == Publisher::BTN1) break; // Leave thread and shut down if button 1 is pressed
	
		// Move forward continuously
		pub.move(100, 0); //100m/s 0mm radius

		// While sensors not tripped, continue. If the are, move on.
		while( (pub.wheelSensor == 0 || pub.wheelSensor >= 3) && (pub.bumperSensor == 0 || pub.bumperSensor >= 5) && (pub.cliffSensor == 0 || pub.cliffSensor >= 5)) {
			printf(" Wheel: %d | Cliff: %d | Bumper: %d | Btn: %d\n", pub.wheelSensor, pub.cliffSensor, pub.bumperSensor, pub.button);

			delay(10);
		}
		// Tell us which sensor tripped
		printf(" Wheel: %d | Cliff: %d | Bumper: %d | Btn: %d\n", pub.wheelSensor, pub.cliffSensor, pub.bumperSensor, pub.button);

		// Reverse then rotate then continue in loop
		pub.moveDist(150, -50, 0);
		delay(200);
		pub.rotate(3.14/4, 3.14/6);
		delay(200);

	}
}
