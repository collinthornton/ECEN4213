
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
//#include <wiringSerial.h>
//#include <wiringPi.h>

#include "joystick.hh"


#define PORT 8000
using namespace std;

void movement(int, int);
void createSocket();
void readData();

int kobuki, new_socket;

/*Create char buffer to store transmitted data*/

int main(){
	//Initialize filestream for the Kobuki
	//wiringPiSetup();
	//kobuki = serialOpen("/dev/kobuki", 115200);

	//Create connection to client

	Joystick joystick("/dev/input/js0");
	printf("Joystick found: %d\r\n", joystick.isFound());


	createSocket();

	JoystickEvent event;
	unsigned int button;

	printf("INITIALIZED JOYSTICK \r\n");

	//The joystick creates events when a button or axis changes value.
	//Sample event from the joystick: joystick.sample(&event)

	//You can interpret these by sampling the events.
	//Each event has three parameters.
	//A type, axis or button,
	//judge if the event is button: event.isButton()
	//judge if the event is axis: event.isAxis()
	//A number corresponding to the axis or button pressed: event.number
	//And a value, Buttons: 0-unpressed, 1-pressed, Axis: -32767 to 0 to 32767: event.value

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
			cout << event << "\t type=" << event.type << " number=" << event.number << " value=" << event.value << endl;

			if (event.isButton())
			{
				//printf("isButton: %u | Value: %d\n", event.number, event.value);
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
				//printf("isAxis: %u | Value: %d\n", event.number, event.value);
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
		char buffer[1024] = {0};

		sprintf(buffer, "%f:%f:%d:", vel, rad, shutdown);
		//cout << buffer << "\tUSE DPAD: " << useDPAD << endl;
		//cout << "isAxis: " << event.isAxis() << "\tnumber: " << event.number << "\tvalue: " << event.value << "\tvel: " << vel << "\trad: " << rad << endl;
		std::this_thread::sleep_for(std::chrono::milliseconds(10));
	}
}

void movement(int sp, int r){
	//Create the byte stream packet with the following format:
	/*Byte 0: Kobuki Header 0*/
	/*Byte 1: Kobuki Header 1*/
	/*Byte 2: Length of Payload*/
	/*Byte 3: Payload Header*/
	/*Byte 4: Payload Data: Length*/
	/*Byte 5: Payload Data: Speed(mm/s)*/
	/*Byte 6: Payload Data: Speed(mm/s)*/
	/*Byte 7: Payload Data: Radius(mm)*/
	/*Byte 8: Payload Data: Radius(mm)*/
	/*Byte 9: Checksum*/

	/*Send the data to the Kobuki over a serial stream*/

	/*Checksum all the data and send that as well*/

	/*Pause the script so the data send rate is the
	same as the Kobuki receive rate*/
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

void readData(){
	/*Read the incoming data stream from the controller*/


	
	/*Reset the buffer*/
	//memset(&/*buffer*/, '0', sizeof(/*buffer*/));

	/*Print the data to the terminal*/


	

	/*Use the received data to control the Kobuki*/

	
	//if() {
	/*Closes out of all connections cleanly*/

	//When you need to close out of all connections, please
	//close both the Kobuki and TTP/IP data streams.
	//Not doing so will result in the need to restart
	//the raspberry pi and Kobuki
		close(new_socket);
		//serialClose(kobuki);
		exit(0);
	//}



}
