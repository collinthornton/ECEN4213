#include "adc.hpp"
#include <wiringPi.h>
#include <wiringPiI2C.h>
#include <unistd.h>

ADC::ADC(int i2cDeviceID) {
    this->adc = wiringPiI2CSetup(i2cDeviceID);
    wiringPiI2CWriteReg16(adc, 0x01, 0x8340);
    sleep(.1);
}

void ADC::run() {
    this->runThread = std::thread(&ADC::runP, this);
}

void ADC::stop() {
    this->stopThread = true;

    if(this->runThread.joinable()) this->runThread.join();
}

void ADC::runP() {

    while(!this->stopThread) {
        usleep(500); //
        std::lock_guard<std::mutex> guard(this->m);
        this->bitVal = this->smoothingFactor*this->readBits() + (1.0-this->smoothingFactor)*prevBitVal;
        this->prevBitVal = this->bitVal;
    }
}

double ADC::readmv() {
    std::lock_guard<std::mutex> guard(this->m);
    double mv = (double)this->bitVal * this->mvConversionFactor;
    return mv;
}

int ADC::readBits() {

	uint16_t low, high, value;

	// Read value at register 0x00 on ADC
    uint16_t data = wiringPiI2CReadReg16(this->adc, 0x00);

    // Bit shift read value's bytes (keeping in mind that its transmitted LSB first) to return to usable form
    low = (data & 0xFF00) >> 8;
    high = (data & 0x00FF) << 8;
    value = (high | low)>>4;
	return value;
}