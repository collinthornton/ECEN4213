/*
Authors:	Max DeSantis, Collin Thornton
Exercise:	Lab3Ex1
Note:		
*/
//use g++ -std=c++11 -o Lab4EX1 Lab4EX1.cpp publisher.cpp -lwiringPi -pthread

#include <iostream>
#include <unistd.h>
#include <wiringSerial.h>
#include <wiringPi.h>
#include "publisher.hpp"
using namespace std;

// We use custom publisher class to abstract sending of data to Kobuki. Requires we only use pub.move, pub.rotate etc.
Publisher pub("/dev/kobuki", 115200);

int main(){
	pub.run();

	printf("\n\n\n");

	/*Starting at bottom right corner, travel
	along bottom side. (You can stop Kobuki before running the every next segment.)*/
	pub.moveDist(1200, 150, 0);
	printf("\n");
	delay(500);
	/*Rotate the Kobuki 90 degrees clockwise*/
	pub.rotate(3.0/2.0, -3.14/6.0);
	delay(500);
	/*Move along left side*/
	pub.moveDist(1200, 150, 0);
	delay(500);
	/*Move along semicircle*/
	pub.moveDist(3.14*550, 100, -500);
	delay(500);
	/*Move along right side*/
	pub.moveDist(1200, 150, 0);
	delay(500);
	/*Rotate the Kobuki 90 degrees clockwise*/
	pub.rotate(3.1415/2.0, -3.1415/6.0);
	delay(500);
	//Close the serial stream to the Kobuki
	//Not doing this will result in an unclean disconnect
	//resulting in the need to restart the Kobuki
	//after every run.
	return(0);
}
