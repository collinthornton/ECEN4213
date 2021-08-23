//In this exercise, you will toggle an LED using a button switch.

//Use the digital I/O pins on the Raspberry Pi to read the state
//of the switch, and send the corresponding signal to the LED.

//Thoroughly comment your code and demonstrate to the lab instructor.


//Use "g++ -std=c++11 -o Lab1EX3 Lab1EX3.cpp -lwiringPi" to compile to code and
// run the code using "./Lab1EX3"

#include <iostream>
#include <unistd.h>
#include <wiringPi.h>
#include <signal.h>
#include <stdlib.h>
using namespace std;


const int BUTTON_PIN = 4;
const int LED_PIN = 1;

int main(){
	//Initialize the wiringPi library.
	wiringPiSetup();

	/*Set digital pins to be output or input.*/
	//hints: pinMode(pin, mode);
	pinMode(LED_PIN, OUTPUT);
	pinMode(BUTTON_PIN, INPUT);

	bool buttonState = 0;

	while(true){
		/*If there is a change in the state of the
		switch, the state of the LED must also change.*/
		//hints: digitalRead(pin) ; 
		//hints: digitalWrite(pin, value); 

		if(digitalRead(BUTTON_PIN))
			digitalWrite(LED_PIN, HIGH);
		else
			digitalWrite(LED_PIN, LOW);

		sleep(0.1);

	}
}

