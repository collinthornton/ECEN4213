#include "adc.hpp"
#include <wiringPi.h>
#include <wiringPiI2C.h>
#include <unistd.h>

ADC::ADC(int i2cDeviceID) {
    // Initialize ADC I2C address on wiring PI
    this->adc = wiringPiI2CSetup(i2cDeviceID);

    // Setup up ADC control registers
    wiringPiI2CWriteReg16(adc, 0x01, 0x8340);
    sleep(.1);
}

void ADC::run() {
    // Run a thread for asynchronous measurements
    this->runThread = std::thread(&ADC::runP, this);
}

void ADC::stop() {
    // Stop the thread. Called by parent process
    this->stopThread = true;

    // Wait for thread to stop
    if(this->runThread.joinable()) this->runThread.join();
}

void ADC::runP() {
    // Private run function

    // Run while loop until told to stop
    while(!this->stopThread) {
        usleep(500); 
        
        // Obtain a lock to access data
        std::lock_guard<std::mutex> guard(this->m);

        // Read the value from the ADC. Smooth with a complementary filter
        this->bitVal = this->smoothingFactor*this->readBits() + (1.0-this->smoothingFactor)*prevBitVal;
        this->prevBitVal = this->bitVal;
    }
}

double ADC::readmv() {
    // Public function to read data

    std::lock_guard<std::mutex> guard(this->m);
    double mv = (double)this->bitVal * this->mvConversionFactor;
    return mv;
}

int ADC::readBits() {
    // Private function to poll ADC

	uint16_t low, high, value;

	// Read value at register 0x00 on ADC
    uint16_t data = wiringPiI2CReadReg16(this->adc, 0x00);

    // Bit shift read value's bytes (keeping in mind that its transmitted LSB first) to return to usable form
    low = (data & 0xFF00) >> 8;
    high = (data & 0x00FF) << 8;
    value = (high | low)>>4;
	return value;
}