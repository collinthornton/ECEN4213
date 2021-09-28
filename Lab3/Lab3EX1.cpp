//Use g++ -std=c++11 -o Lab3EX1 Lab3EX1.cpp -lwiringPi

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

SONAR sonar(sigPin);


int main(){
	sonar.run();

	while(true) {
		printf("Distance: %6.3f\tAlpha: %6.3f\tPrev: %6.3f\n", sonar.read(), sonar.smoothingFactor, sonar.prevDistance);
		delay(10);
	}

}
