/*
AUTHORS:	Max DeSantis, Collin Thornton
EXERCISE:	Lab 1 - EX4
*/

#include <iostream>
#include <iomanip>
#include <unistd.h>
#include <wiringPi.h>
#include <wiringPiI2C.h>
#include <signal.h>
#include <stdlib.h>
using namespace std;



int adcVal(int a);

int main(){
	// Initialize the wiringPi Library.
	wiringPiSetup();
	
	// Initialize the wiringPi I2C interface
	int adc = wiringPiI2CSetup(0x48);

	// Loop continously
	while(true){
		/*Read value from ADC*/
		int bitVal = adcVal(adc);

		/*Convert data to milliVolts.*/
		int mVolts =bitVal * 6144 / 2047;

		/*Print value to screen.*/
		printf("mV = %d\r\n", mVolts);

		/*Slow the script speed down to gain better measurements.*/
		sleep(0.1);
	}
}

//This function is used to read data from ADC1015
int adcVal(int adc){

	uint16_t low, high, value;
	// Write value of 0x8340 to register 0x01 (Actually using 0x4083, but LSB first)
	wiringPiI2CWriteReg16(adc, 0x01, 0x8340);
	usleep(1000);
	
	// Read value at register 0x00 on ADC
    	uint16_t data = wiringPiI2CReadReg16(adc, 0x00);

	// Bit shift read value's bytes (keeping in mind that its transmitted LSB first) to return to usable form
    	low = (data & 0xFF00) >> 8;
    	high = (data & 0x00FF) << 8;
    	value = (high | low)>>4;
	
	return value;
}
