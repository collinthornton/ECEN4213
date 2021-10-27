/*
Authors:	Max DeSantis, Collin Thornton
Exercise:	Lab4 EX 2 - Joystick control
*/

//Use g++ joystick.cc -std=c++11 -o Lab4EX2 Lab4EX2.cpp -lwiringPi

#include <iostream>
#include <wiringPi.h>
#include <wiringSerial.h>
#include "joystick.hh"
#include <unistd.h>
#include <cstdlib>
#include "publisher.hpp"
using namespace std;

// Publisher abstracts sending of commands to Kobuki
Publisher pub("/dev/kobuki", 115200);

int main(){
	pub.run();

	printf("\n\n\n");

	// Axis 0 -> left/right left stick
	//	[-32767,32767] (right positive)
	// Axis 1 -> up/down left stick
	//	[-32767,32767] (down positive)

	// Axis 3 -> left/right right stick
	//	[-32767,32767] (right positive)

	// Axis 4 -> up/down right stick
	//	[-32767,32767] (down positive)

	// Dpad
	// 	Axis 6 -> left/right (-32767 on left)
	//  Axis 7 -> up/down (-32767 on up)

	// Button 7 -> start
	// Button 8 -> Select (Logitech)


	Joystick joystick("/dev/input/js0");
	printf("Joystick found: %d\r\n", joystick.isFound());

	JoystickEvent event;
	unsigned int button;

	printf("INITIALIZED JOYSTICK \r\n");

	float rad = 0;
	float vel = 0;

	bool shutdown = false;
	bool useDPAD = true;

	while(!shutdown){
		/*Create a series of commands to interpret the
		joystick input and use that input to move the Kobuki*/

		//Use the following Key Map:
		//Up     - move the Kobuki forward
		//Down   - move the Kobuki backward
		//Left   - rotate the Kobuki 90 degrees counterclockwise
		//Right  - rotate the Kobuki 90 degrees clockwise
		//Start  - immediately stop the Kobuki's movement
		//Select - exit the script and close the Kobuki's connection cleanly		
		if (joystick.sample(&event))
		{
			if (event.isButton())
			{
				/*Interpret the joystick input and use that input to move the Kobuki*/
				switch(event.number) {
					case 8: // LOGITECH
						if(event.value != 0) {
							printf("\nSHUTTING DOWN\n");
							shutdown = true;
						}
						break;

					case 0: 
						if(event.value != 0) {
							useDPAD = !useDPAD;
						}
						break;
				}

			}
			if (event.isAxis())
			{
				/*Interpret the joystick input and use that input to move the Kobuki*/

				switch(event.number) {
					case 6:	// DPAD left/right
						if (!useDPAD) break;

						rad = event.value > 0 ? -1 : 1;
						vel = event.value != 0 ? 3.1415 : 0;
						break;

					case 7:	// DPAD up/down
						if (!useDPAD) break;

						rad = 0;
						if(event.value == 0) vel = 0;
						else if(event.value > 0) vel = -500;
						else vel = 500;

						break;
					
					// Right analog stick. Linearly maps commands from joystick to velocity and radius, allowing for smooth control. BONUS
					case 3:
						if(useDPAD) break;
						if (event.value == 0) rad = 0;
						else if(event.value == -32767) rad = -1;
						else if(event.value == 32767) rad = 1;
						else if(event.value > 0) rad = (500.0-1.0)/32767.0*event.value - 500.0;
						else if(event.value < 0) rad = (500.0-1.0)/32767.0*event.value + 500.0;
						break;

					case 4:
						if(useDPAD) break;

						vel = -(float)event.value/32767.0 * 500;
						break;
				}
				
			}
		}
		
		// Send command every loop.
		printf("DPAD: %d      MOVE AT (%5.2f mm/s, %5.2f m)\r                        ", useDPAD, vel, rad);
		pub.move(vel,rad);
		delay(10);
	}

	return 0;
}
