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
int sonarPin = #;
void movement(int, int);

Publisher pub("/dev/kobuki", 115200);
SONAR sonar(sonarPin);

int main(){

	/*Move from a random point within the area designated "X" to the
	point B as shown on the diagram. Use a sonar sensor to navigate through the channel.
	You can reuse your code from Lab 3 and 4*/

	/*Note: the Kobuki must completely pass point B as shown to receive full credit*/


}