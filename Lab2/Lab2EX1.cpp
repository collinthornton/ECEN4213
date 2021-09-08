/*
Authors:    Max DeSantis, Collin Thornton
Exercise:   Lab2 EX 1
*/
// g++ -std=c++11 -o Lab2EX1 Lab2EX1.cpp -lwiringPi

#include <wiringPi.h>
#include <softPwm.h>
#include <stdio.h>



/* signal pin of the servo*/
#define servoPin    1

//Specific a certain rotation angle (0-180) for the servo
void servoWrite(int pin, int angle){
    int time = 5 + angle / 9;
    softPwmWrite(pin,time);   
}

int main(void)
{
    wiringPiSetup();    
    softPwmCreate(servoPin,  0, 200);
    servoWrite(servoPin, 0);


    while(1){
        delay(400);

        for(int i = 0; i < 180; i++) {
            servoWrite(servoPin, i);
            delay(10);
        }

        delay(500);

        for(int i = 180; i > 0; i--) {
            servoWrite(servoPin, i);
            delay(10);
        }
    } 
    return 0;
}

