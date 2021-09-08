
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
    
    wiringPiSetup();

    /*1. set mode for the three pins, the pin mode of the PWM pin should be PWM_OUTPUT*/
    pinMode(pwmPin, PWM_OUTPUT);
    pinMode(en0, OUTPUT);
    pinMode(en1, OUTPUT);
	/*2. write HIGH or LOW to the two I/O pins to control the rotation direction*/
    digitalWrite(en0, HIGH);
    digitalWrite(en1, LOW);
    /*3. use function pwmWrite to control the motor speed. Refer to document pwm.pdf for more information.*/
    pwmWrite(pwmPin, 1024);
    delay(5000);
    pwmWrite(pwmPin, 0);
    return 0;
}

