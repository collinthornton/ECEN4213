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
	int num = rand() % 10 + 1;
	printf(" %d", num);
	return 0;
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

		delayTime = (rand() % 10 + 1) * 1000; // Delay in ms
		pub.move(100, 0); //100m/s 0mm radius
		startTime = millis();
		while(millis() - startTime < delayTime) {
			// Check sensors here
		}

		delayTime = (rand() % 5 + 1) * 1000;
		pub.rotate(/* Compute omega here*/, /* Randomize direction here*/)
		startTime = millis();
		while(millis() - startTime < delayTime) {
			// Check sensors here
		}

		// Anything left before restarting?
	}
}
