/*
AUTHORS:	Max DeSantis, Collin Thornton
EXERCISE:	Lab1 - Ex3
*/

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
	pinMode(LED_PIN, OUTPUT);
	pinMode(BUTTON_PIN, INPUT);
	
	// Loop continuously
	while(true){
		// If the button is pressed, turn the LED on (HIGH)
		if(digitalRead(BUTTON_PIN))
			digitalWrite(LED_PIN, HIGH);
		// If the button is unpressed, turn the LED off (LOW)
		else
			digitalWrite(LED_PIN, LOW);
		
		// Pause briefly between loops
		sleep(0.1);

	}
}

