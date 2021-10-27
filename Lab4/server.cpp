/*
Authors:	Max DeSantis, Collin Thornton
Exercise:	Server component of Lab4 EX3
Note:		Ran on side computer with Joystick. Client runs on Pi.
*/

//Use g++ -std=c++11 -o server server.cpp joystick.cc -lwiringPi

#include <iostream>
#include <iomanip>
#include <unistd.h>
#include <cstdlib>
#include <stdio.h>
#include <string.h>
#include <string>
#include <sys/socket.h>
#include <netinet/in.h>

#include <chrono>
#include <iostream>
#include <thread>

#include "joystick.hh"


#define PORT 8000
using namespace std;

void movement(int, int);
void createSocket();
void readData();

int kobuki, new_socket;

/*Create char buffer to store transmitted data*/

int main(){

	// Create joystick
	Joystick joystick("/dev/input/js0");
	printf("Joystick found: %d\r\n", joystick.isFound());

	// Create connection to client
	createSocket();

	JoystickEvent event;
	unsigned int button;

	printf("INITIALIZED JOYSTICK \r\n");

	float rad = 0;
	float vel = 0;

	bool shutdown = false;
	bool useDPAD = true;

	int value = static_cast<int>(event.value);
	int number = static_cast<int>(event.number);
	
	while(!shutdown){
		/*Create a series of commands to interpret the
		joystick input and use that input to move the Kobuki*/
		
		// On every event, check if it is a button or axis
		if (joystick.sample(&event))
		{
			value = static_cast<int>(event.value);
			number = static_cast<int>(event.number);

			// If event is a button, check if it was the shutdown button or DPAD enable button
			if (event.isButton())
			{
				switch(number) {
					case 8: // LOGITECH. Shutdown Kobuki.
						if(value != 0) {
							printf("\nSHUTTING DOWN\n");
							shutdown = true;
						}
						break;

					case 0: // Switch between Analog stick and DPAD
						if(value != 0) {
							useDPAD = !useDPAD;
						}
						break;
				}

			}
			// If event is an axis, determine a radius and velocity from joystick commands.
			if (event.isAxis())
			{
				//printf("isAxis: %u | Value: %d\n", number, value);
				/*Interpret the joystick input and use that input to move the Kobuki*/

				switch(number) {
					case 6:	// DPAD left/right
						if (!useDPAD) break;

						rad = value > 0 ? -1 : 1;
						vel = value != 0 ? 3.1415 : 0;
						break;

					case 7:	// DPAD up/down
						if (!useDPAD) break;

						rad = 0;
						if(value == 0) vel = 0;
						else if(value > 0) vel = -500;
						else vel = 500;

						break;
					// Right analog stick. Takes input and makes a linear mapping allowing for smooth control of forward velocity and rotation. BONUS
					case 2:
						if(useDPAD) break;
						if (value == 0) rad = 0;
						else if(value == -32767) rad = -1;
						else if(value == 32767) rad = 1;
						else if(value > 0) rad = (500.0-1.0)/32767.0*value - 500.0;
						else if(value < 0) rad = (500.0-1.0)/32767.0*value + 500.0;
						break;

					case 3:
						if(useDPAD) break;

						vel = -(float)value/32767.0 * 500;
						break;
				}
				
			}
			
			// Assemble message and transmit to client
			char buffer[1024] = {0};

			string die = shutdown ? "DIE" : "LIVE";

			sprintf(buffer, "%f:%f:%s:", vel, rad, die.c_str());
			send(new_socket, buffer, strlen(buffer), 0);
			cout << buffer << " USE DPAD: " << useDPAD << endl;
		}

		std::this_thread::sleep_for(std::chrono::milliseconds(10));
	}
}

//Creates the connection between the client and
//the server with the Kobuki being the server
void createSocket(){
	int server_fd;
	struct sockaddr_in address;
	int opt =1;
	int addrlen = sizeof(address);

	if((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0){
		perror("socket failed");
		exit(EXIT_FAILURE);
	}

	if(setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))){
		perror("setsockopt");
		exit(EXIT_FAILURE);
	}

	address.sin_family      = AF_INET;
	address.sin_addr.s_addr = INADDR_ANY;
	address.sin_port        = htons(PORT);

	if(bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0){
		perror("bind failed");
		exit(EXIT_FAILURE);
	}

	if(listen(server_fd, 3) < 0){
		perror("listen");
		exit(EXIT_FAILURE);
	}

	if((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen)) < 0){
		perror("accept");
		exit(EXIT_FAILURE);
	}
}
