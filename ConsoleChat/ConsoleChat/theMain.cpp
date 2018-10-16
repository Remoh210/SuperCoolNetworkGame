#define WIN32_LEAN_AND_MEAN
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <iostream>
#include <vector>
#include "ConnectionMaintainer.h"
#pragma comment(lib, "Ws2_32.lib")
#pragma comment(lib, "Mswsock.lib")
#pragma comment(lib, "AdvApi32.lib")
#define BUFFER_LENGTH 512
#define PORT_NUMBER "5000"
#define SERVER_ADDRESS "127.0.0.1"
#define ARRAY_SIZE 128  

char Answer;

int main() {
	UserInfo User;

	std::cout << "Connect to the loacal server? \n";
	std::cout << "Type \"y\" or \"n\": ";
	std::cin >> Answer;


	if (Answer == 'y' || Answer == 'Y') {
		std::cout << "Connecting to the localhost \n";
		User.ServerAdress = SERVER_ADDRESS;
	}
	else if (Answer == 'n' || Answer == 'N') {
		char serverAdress[ARRAY_SIZE];
		std::cout << "Specify the adress: \n";
		std::cin >> serverAdress;
		User.ServerAdress = serverAdress;
	}
	else {
		std::cout << "Incorrect answer\n";
	};

	std::cout << "Enter Your Name: \n";
	char FirstName[ARRAY_SIZE];
	std::cin >> FirstName;
	User.Name = FirstName;

	// Buffer send
	Buffer* Send_Buffer = new Buffer(BUFFER_LENGTH);

	// Buffer recieve
	Buffer* Recieve_Buffer = new Buffer(BUFFER_LENGTH);

	ConnectionMaintainer Conn;
	Conn.Connect(User);


	return 0;
}