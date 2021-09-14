/*
Authors:    Max DeSantis, Collin Thornton
Exercise:   Lab 2 EX 5
*/

// g++ -std=c++14 Lab2EX5.cpp -o Lab2EX5 -lwiringPi


#include <stdio.h>
#include <wiringPi.h>
#include <wiringPiI2C.h>
#include <string.h>
#include <iostream>

using namespace std;

/* Complete the code: I2c address of LCD, some LCD i2c address might be 0x27 */
int LCDAddr = 0x27;  


int BLEN = 1; // 1--open backlight.0--close backlight
int fd; // Linux file descriptor
bool dirState = false;
bool spdState = false;
int speed = 200;
int lastDirChange = 0;
int lastSpeedChange = 0;
int spdStep = 20;

int en0 = 2;
int en1 = 3;
int pwmPin = 26;
int dirBtn = 1;
int spdBtn = 4;


// Send an 16 bits data to LCD buffer
void write_word(int data){
    int temp = data;
    if ( BLEN == 1 )
        temp |= 0x08;
    else
        temp &= 0xF7;
    wiringPiI2CWrite(fd,temp);

}

// Send control command to lcd
void send_command(int comm){
    int buf;
    // Send bit7-4 firstly
    buf = comm & 0xF0;
    buf |= 0x04;            // RS = 0, RW = 0, EN = 1
    write_word(buf);
    delay(2);
    buf &= 0xFB;            // Make EN = 0
    write_word(buf);
    // Send bit3-0 secondly
    buf = (comm & 0x0F) << 4;
    buf |= 0x04;            // RS = 0, RW = 0, EN = 1
    write_word(buf);
    delay(2);
    buf &= 0xFB;            // Make EN = 0
    write_word(buf);
}

// Send character to lcd
void send_data(int data){
    int buf;
    // Send bit7-4 firstly
    buf = data & 0xF0;
    buf |= 0x05;            // RS = 1, RW = 0, EN = 1
    write_word(buf);
    delay(2);
    buf &= 0xFB;            // Make EN = 0
    write_word(buf);
    // Send bit3-0 secondly
    buf = (data & 0x0F) << 4;
    buf |= 0x05;            // RS = 1, RW = 0, EN = 1
    write_word(buf);
    delay(2);
    buf &= 0xFB;            // Make EN = 0
    write_word(buf);
}

// Initialize the lcd
void init(){
    send_command(0x33); // Initialize 
    delay(5);
    send_command(0x32); // Initialize 
    delay(5);

    /* Complete the code: 4bit mode 2 Lines & 5*8 dots */
    send_command(0x28);
    delay(5);

    send_command(0x0C); // Enable display without cursor
    delay(5);

    /* Complete the code: Clear screen */
    send_command(0x01); 
    
}

// Clear screen
void clear(){
    /* Complete the code:clear Screen */
    send_command(0x01);

}

// Print the message on the lcd
void write(int x, int y, const char data[]){
    int addr, i;
    int tmp;
    if (x < 0) x = 0; if (x > 15) x = 15;
    if (y < 0) y = 0; if (y > 1)  y = 1;

    /* Complete the code: Target address, Move cursor */
    addr = 0x80 + 0x40*y + x;

    // Set the address
    send_command(addr);
    
    tmp = strlen(data);
    for (i = 0; i < tmp; i++){
        send_data(data[i]);
    }
}

void print_info()
{
    printf("\n");
    printf("|***************************|\n");
    printf("|    IIC 1602 LCD test      |\n");
    printf("| --------------------------|\n");
    printf("| | LCD |            | Pi   |\n");
    printf("| --------------------------|\n");
    printf("| | GND | connect to | GND  |\n");
    printf("| | VCC | connect to | 5V   |\n");
    printf("| | SDA | connect to | SDA.1|\n");
    printf("| | SCL | connect to | SCL.1|\n");
    printf("| --------------------------|\n");
    printf("|                   ECEN4213|\n");
    printf("|***************************|\n");
    printf("Program is running...\n");
    printf("Press Ctrl+C to end the program\n");
}

void set_dir_callback() {
    if(millis() - lastDirChange < 200) {
        return;
    }
    
    lastDirChange = millis();
    if(digitalRead(dirBtn)) {
        printf("DirState: %d\r\n", dirState);
        dirState = !dirState;
        digitalWrite(en0, dirState);
        digitalWrite(en1, !dirState);
    }

}

void speed_callback() {

    
    if(millis() - lastSpeedChange < 100) {
        return;
    }
    lastSpeedChange = millis();

    if(digitalRead(spdBtn)) {
        speed += spdStep;
        if(speed >= 1024) {
            spdStep = -20;
            speed = 1020;
        }
        else if(speed <= 200){
            spdStep = 20;
            speed = 200;
        }
    }
    
}

int main(){

    // Set up WiringPi
    if (wiringPiSetup () == -1) exit (1);    
    
    // Init I2C, assign a buffer handler to variable fd
    fd = wiringPiI2CSetup(LCDAddr);

    /* Complete the code: in the init function*/
    init();


    print_info();

    /* Set up two interrupts using wiringPiISR() for two buttons. You can refer to the lab documents.
    You can use some global variables which can be used in the following while loop*/
    pinMode(en0, OUTPUT);
    pinMode(en1, OUTPUT);
    pinMode(pwmPin, PWM_OUTPUT);
    pinMode(dirBtn, INPUT);
    pinMode(spdBtn, INPUT);
    wiringPiISR(dirBtn, INT_EDGE_RISING, &set_dir_callback);
    wiringPiISR(spdBtn, INT_EDGE_RISING, &speed_callback);
    int lastUpdate = 0;
    char buf[16];
    lastDirChange = lastSpeedChange = lastUpdate = millis();
    write(0, 0, "DR:");
    write(0, 1, "Speed:");

    while(true) {
        /*According to the global variables, the LCD display and DC motor speed and direction can be controlled*/

        pwmWrite(pwmPin, speed);

        dirState ? write(4, 0, "CW   ") : write(4, 0, "CCW   ");
        sprintf(buf, "%d    ", speed);
        write(7, 1, buf);

        /*
        delay(max((unsigned int)0,100-(millis()-lastUpdate)));
        lastUpdate = millis();
        */
        delay(100);
    }

    return 0;
}

