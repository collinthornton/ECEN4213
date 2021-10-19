#include "publisher.hpp"
#include <wiringSerial.h>
#include <wiringPi.h>
#include <thread>
#include <cmath>

Publisher::Publisher(string serialPort, int baudRate) {
    wiringPiSetup();
    this->serialPort = serialPort;
    this->baud = baudRate;
    this->kobukiId = serialOpen(serialPort.c_str(), baudRate);
    this->stopThread = false;
}

Publisher::~Publisher() {
    this->stopThread = true;
    if(this->runThread.joinable()) {
        this->runThread.join();
    }
}

void Publisher::run() {
    this->runThread = std::thread(&Publisher::runP, this);
}

void Publisher::stop() {
    this->stopThread = true;
    if(this->runThread.joinable()) {
        this->runThread.join();
    }
}

void Publisher::runP() {
    while(!this->stopThread) {
        sendMovement();
        delay(20);
    }
    this->speed = 0;
    this->radius = 0;
    sendMovement();
    serialClose(this->kobukiId);
}

void Publisher::sendMovement() {
    //Create the byte stream packet with the following format:
    unsigned char b_0 = 0xAA; /*Byte 0: Kobuki Header 0*/
    unsigned char b_1 = 0x55; /*Byte 1: Kobuki Header 1*/
    unsigned char b_2 = 0x06; /*Byte 2: Length of Payload*/
    unsigned char b_3 = 0x01; /*Byte 3: Sub-Payload Header*/
    unsigned char b_4 = 0x04; /*Byte 4: Length of Sub-Payload*/

    unsigned char b_5 = this->speed & 0xff;	//Byte 5: Payload Data: Speed(mm/s)
    unsigned char b_6 = (this->speed >> 8) & 0xff; //Byte 6: Payload Data: Speed(mm/s)
    unsigned char b_7 = this->radius & 0xff;	//Byte 7: Payload Data: Radius(mm)
    unsigned char b_8 = (this->radius >> 8) & 0xff;	//Byte 8: Payload Data: Radius(mm)
    unsigned char checksum = 0;		//Byte 9: Checksum
    
    //Checksum all of the data
    char packet[] = {b_0,b_1,b_2,b_3,b_4,b_5,b_6,b_7,b_8};
    for (unsigned int i = 2; i < 9; i++)
        checksum ^= packet[i];
        
    /*Send the data (Byte 1 - Byte 9) to Kobuki using serialPutchar (kobuki, );*/
    for(int i = 0; i < 9; i++)
        serialPutchar(this->kobukiId, packet[i]);
    serialPutchar(this->kobukiId, checksum);

}

void Publisher::moveDist(float distance, float velocity, float radius) {
    int speed;
    if(radius > 1) speed = velocity * (radius + 230 / 2) / radius;
    else if(radius < -1) speed = velocity * (radius - 230 / 2) / radius;
    else speed = velocity;
    this->radius = radius;
    this->speed = speed;
    int start = millis();
    while(millis() - start <= (distance / velocity) * 1000) {
        printf("Time: %5.2d/%5.2d\r", millis()-start, (int)(distance/velocity* 1000));
        delay(1);
    }

    this->radius = 0;
    this->speed = 0;
}

void Publisher::move(float velocity, float radius) {
    int speed;
    if(radius >= 1) speed = velocity * (radius + 230 / 2) / radius;
    else if(radius <= -1) speed = velocity * (radius - 230 / 2) / radius;
    else speed = velocity;
    this->radius = radius;
    this->speed = speed;   
}

void Publisher::rotate(float theta, float omega) {
    int radius;
    if(omega >0) {
        radius = 1;
    }
    else if(omega <0) {
        radius = -1;
        omega = std::abs(omega);
    }
    else {
        return;
    }
    int speed = omega * 90.0/75.0*310.0 / 2.0;
    this->speed = speed;
    this->radius = radius;

    int start = millis();
    while(millis() - start <= (theta/std::abs(omega))*1000) {
        printf("Time: %5.2d/%5.2d\r", millis()-start, (int)(theta/std::abs(omega)* 1000));
        delay(1);
    }
    this->speed = 0;
    this->radius = 0;
}