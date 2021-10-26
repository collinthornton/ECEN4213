
//Use g++ -std=c++11 -o client client.cpp joystick.cc publisher.cpp -lpthread -lwiringPi


#include <stdio.h>
#include <iostream>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <unistd.h>
#include <string>
#include "joystick.hh"
#include <sstream>
#include "publisher.hpp"
#include <cstdlib>
#define  PORT 8000
#define BUFFERSIZE 1024
using namespace std;

int createSocket();
struct data parseData(int valread, string data);

struct data {
	float rad;
	float vel;
	bool shutdown;
};

int sock = 0;
Publisher pub("/dev/kobuki", 115200);

int main(int argc, char const *argv[]){
	

	//Create the connection to the server
	createSocket();
	pub.run();
	printf("\n\n");

	struct data newData;

	while(true){

        char buffer[BUFFERSIZE] = {0};
        int valread = read(sock, buffer, BUFFERSIZE);
        //cout << "valread: " << valread << "| val: " << buffer << endl;
		newData = parseData(valread, string(buffer));
		printf("vel: %f | rad: %f | sd: %d\n", newData.vel, newData.rad, newData.shutdown);
		if(newData.shutdown) break;
		pub.move(newData.vel, newData.rad);
		
	}
	return 0;
}

struct data parseData(int valread, string data) {
	struct data d;
	d.vel = 0;
	d.rad = 0;
	d.shutdown = 0;

	// Format %f:%f:%d (vel, rad, shutdown)
	if(valread < 0) return d;
	stringstream data_stream(data);
	string temp;
	
	getline(data_stream, temp, ':');
	d.vel = stof(temp);
	getline(data_stream, temp, ':');
	d.rad = stof(temp);
	getline(data_stream, temp, ':');
	d.shutdown = (temp == "DIE") ? 1 : 0;

	return d;
}

//Creates the connection between the client and
//the server with the controller being the client
int createSocket(){
	struct sockaddr_in address;
	struct sockaddr_in serv_addr;

	if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0){
		printf("\nSocket creation error \n");
		return -1;
	}
	
	memset(&serv_addr, '0', sizeof(serv_addr));

	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port   = htons(PORT);

	/*Use the IP address of the server you are connecting to*/
	if(inet_pton(AF_INET, "10.227.80.20" , &serv_addr.sin_addr) <= 0){
		printf("\nInvalid address/ Address not supported \n");
		return -1;
	}

	if(connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0){
		printf("\nConnection Failed \n");
		return -1;
	}
	return 0;
}

