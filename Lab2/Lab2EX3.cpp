/*
Authors:	Max DeSantis, Collin Thornton
Exercise:	Lab 2 EX 3
*/

#include <wiringPi.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
using namespace std;


// g++ -std=c++14 Lab2EX3.cpp -o Lab2EX3 -lwiringPi

const int pwmPin = 26;
const int en0 = 2;
const int en1 = 3;


int main(void){
    
	// Init
    wiringPiSetup();
    pinMode(pwmPin, PWM_OUTPUT);
    pinMode(en0, OUTPUT);
    pinMode(en1, OUTPUT);
	
	// Set motor direction
    digitalWrite(en0, HIGH);
    digitalWrite(en1, LOW);
    
	// Set motor speed
    pwmWrite(pwmPin, 1024);
	
	//Wait then turn off
    delay(5000);
    pwmWrite(pwmPin, 0);
    return 0;
}

