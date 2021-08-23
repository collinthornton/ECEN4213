//In this exercise, you will be measuring the analog voltage
//emitted by the potentiometer and displaying that value
//on the terminal screen.

//Because the Raspberry Pi lacks an internal ADC, we will use
//the Adafruit ADS1015 12 Bit ADC.

//The ADS1015 communicates with the Raspberry Pi through
//I2C communication protocol.

//Complete the given function and use it to measure the
//voltage emitted by the potentiometer.

//Reference the lab supplement ads1015.pdf and I2CProtocol.pdf
//for more details on the Adafruit ADS1015 12 Bit converter
//and I2C protocol.

// single-end, 0-6.144V

//Use g++ -std=c++11 -o Lab1EX4 Lab1EX4.cpp -lwiringPi

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
	//Initialize the wiringPi Library.
	wiringPiSetup();
	int adc = wiringPiI2CSetup(0x48);


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
	// Refer to the supplemental documents to find the parameters. In this lab, the ADS1015
	// needs to be set in single-end mode, FSR (full-scale range)is 6.144, you can choose 
	// any input pin (A0, A1, A2, A3) you like.
	wiringPiI2CWriteReg16(adc, 0x01, 0x8340);
	usleep(1000);
    uint16_t data = wiringPiI2CReadReg16(adc, 0x00);


    low = (data & 0xFF00) >> 8;
    high = (data & 0x00FF) << 8;
    value = (high | low)>>4;
	return value;
}