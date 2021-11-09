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

int kobuki;
int sonarPin = 1;
void movement(int, int);

Publisher pub("/dev/kobuki", 115200);
SONAR sonar(sonarPin);

int main(){
	sonar.run();
	pub.run();

	delay(500);

	// while(pub.button != Publisher::BTN0) {
	// 	printf("Sonar: %f | Wheel: %d | Cliff: %d | Bumper: %d | Btn: %d\n", sonar.read(), pub.wheelSensor, pub.cliffSensor, pub.bumperSensor, pub.button);
	// 	delay(10);
	// }

	/*Move from a random point within the area designated "X" to the
	point B as shown on the diagram. Use a sonar sensor to navigate through the channel.
	You can reuse your code from Lab 3 and 4*/

	/*Note: the Kobuki must completely pass point B as shown to receive full credit*/

	pub.move(400, 0);
	float dist = sonar.read();
	dist = sonar.read();

	printf("Sonar: %f | Wheel: %d | Cliff: %d | Bumper: %d | Btn: %d\n", sonar.read(), pub.wheelSensor, pub.cliffSensor, pub.bumperSensor, pub.button);

	while(dist >= 25) {
		printf("Sonar: %f | Wheel: %d | Cliff: %d | Bumper: %d | Btn: %d\n", sonar.read(), pub.wheelSensor, pub.cliffSensor, pub.bumperSensor, pub.button);

		dist = sonar.read();
		delay(10);
	}
	pub.move(0, 0);
	delay(200);

	pub.rotate(3.145 / 2.35, -3.145 / 6);
	pub.move(0, 0);
	delay(200);

	pub.move(400, 0);
	while(sonar.read() >= 25) {
		printf("Sonar: %f | Wheel: %d | Cliff: %d | Bumper: %d | Btn: %d\n", sonar.read(), pub.wheelSensor, pub.cliffSensor, pub.bumperSensor, pub.button);

		delay(10);
	}

	pub.move(0, 0);
	delay(200);
	
	pub.rotate(3.145 / 2.35, 3.145 / 6);
	pub.move(0, 0);
	delay(200);

	pub.moveDist(2500, 400, 0);

	delay(1000);
}