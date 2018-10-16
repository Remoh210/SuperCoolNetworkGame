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
void SwitchRoom(UserInfo& cData);

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


	if (Conn.isAlive) {
		cout << "connected!\n"
			<< "-j -l Join and Leave";
		system("pause");
	}
	else {
		cout << "Server is down.\n";
		Sleep(6000);
	}


	string Send_Message;
	string Recieve_Message;
	string ChatBuffer;
	vector<string> ConnRooms;


	while (Conn.isAlive) {
		Recieve_Message = Conn.getMessages();

		// Checking messege from the server
		if (Recieve_Message != "") {
			ChatBuffer += Recieve_Message;

			cout << ChatBuffer;
			cout << "\n";
			cout << "-j means join, -l means leave\n";
			cout << "Use the Enter key to Update\n";
			cout << "Connected to ";
			for (int i = 0; i < ConnRooms.size(); i++) cout << ConnRooms[i] << " ";
			cout << '\n';
			cout << "\n";
		}

		getline(cin, Send_Message);

		if (Send_Message == "-j") {
			Send_Message = "";
			SwitchRoom(User);
			ConnRooms.push_back(User.Room);
			Conn.sendMessage(Send_Buffer, User, MSG_ID_JOIN_ROOM, Send_Message);
		}
		if (Send_Message == "-l") {
			Send_Message = "";
			SwitchRoom(User);
			for (int i = 0; i < ConnRooms.size(); i++) {
				if (ConnRooms.at(i) == User.Room) ConnRooms.at(i).erase();
			}
			Conn.sendMessage(Send_Buffer, User, MSG_ID_LEAVE_ROOM, Send_Message);
		}
		if (Send_Message != "") {
			Conn.sendMessage(Send_Buffer, User, MSG_ID_LEAVE_THE_MESSAGE, Send_Message);
		}
	}


	return 0;
}

void SwitchRoom(UserInfo& cData) {
	cout << "Switch the room by typing its number: \n";
	char* answer;
	answer = new char[ARRAY_SIZE];
	cin >> answer;
	cData.Room = answer;
}