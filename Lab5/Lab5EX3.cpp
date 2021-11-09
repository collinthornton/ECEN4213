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

int kobuki;

unsigned int bumper;
unsigned int drop;
unsigned int cliff;
unsigned int button;

void movement(int, int);
void readData();


Publisher pub("/dev/kobuki", 115200);

int main(){
	pub.run();
	srand(time(NULL));


	// Pseudocode
	/*
	Move a random distance
	Rotate a random amount
	repeat

	While moving, continuously check sensors. If a sensor activates, move backwards until it no longer activates.
	Rotate a random amount.
	Move a random distance forward.

	*/

	unsigned int delayTime;
	unsigned int startTime;
	
	while(true) {
		if(pub.button == Publisher::BTN1) break; // Leave thread and shut down if button 1 is pressed

		pub.move(100, 0); //100m/s 0mm radius

		while( (pub.wheelSensor == 0 || pub.wheelSensor >= 3) && (pub.bumperSensor == 0 || pub.bumperSensor >= 5) && (pub.cliffSensor == 0 || pub.cliffSensor >= 5)) {
			printf(" Wheel: %d | Cliff: %d | Bumper: %d | Btn: %d\n", pub.wheelSensor, pub.cliffSensor, pub.bumperSensor, pub.button);

			delay(10);
		}
		printf(" Wheel: %d | Cliff: %d | Bumper: %d | Btn: %d\n", pub.wheelSensor, pub.cliffSensor, pub.bumperSensor, pub.button);

		pub.moveDist(150, -50, 0);
		delay(200);
		pub.rotate(3.14/4, 3.14/6);
		delay(200);

		// delayTime = (rand() % 10 + 1) * 1000; // Delay in ms
		// pub.rotate(/* Compute omega here*/, /* Randomize direction here*/)
		// startTime = millis();
		// while(millis() - startTime < delayTime) {
		// }

		// Anything left before restarting?
	}
}
