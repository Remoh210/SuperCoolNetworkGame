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
	std::cout << " \n\
:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::" << std::endl << "\
:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::" << std::endl << "\
:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::" << std::endl << "\
:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::" << std::endl << "\
::::::::::::::::##############                              :::::::::::::::::::" << std::endl << "\
############################  ##############################  :::::::::::::::::" << std::endl << "\
#########################  ######???????????????????????######  :::::::::::::::" << std::endl << "\
=========================  ####??????????()????()?????????####  :::::::::::::::" << std::endl << "\
=========================  ##????()??????????????    ()?????##  ::::    :::::::" << std::endl << "\
------------=============  ##??????????????????  ;;;;  ?????##  ::  ;;;;  :::::" << std::endl << "\
-------------------------  ##??????????()??????  ;;;;;;?????##    ;;;;;;  :::::" << std::endl << "\
-------------------------  ##??????????????????  ;;;;;;         ;;;;;;;;  :::::" << std::endl << "\
++++++++++++-------------  ##??????????????????  ;;;;;;;;;;;;;;;;;;;;;;;  :::::" << std::endl << "\
+++++++++++++++++++++++++  ##????????????()??  ;;;;;;;;;;;;;;;;;;;;;;;;;;;  :::" << std::endl << "\
+++++++++++++++++    ;;;;  ##??()????????????  ;;;;;;@@  ;;;;;;;;@@  ;;;;;  :::" << std::endl << "\
~~~~~~~~~~~~~++++;;;;;;;;  ##????????????????  ;;;;;;    ;;;  ;;;    ;;;;;  :::" << std::endl << "\
~~~~~~~~~~~~~~~  ;;  ~~~~  ####??????()??????  ;;[];;;;;;;;;;;;;;;;;;;;;[]  :::" << std::endl << "\
$$$$$$$$$$$$$~~~~  ~~~~~~  ######?????????????  ;;;;;;              ;;;;  :::::" << std::endl << "\
$$$$$$$$$$$$$$$$$$$$$$$$$    ###################  ;;;;;;;;;;;;;;;;;;;;  :::::::" << std::endl << "\
$$$$$$$$$$$$$$$$$$$$$$$  ;;;;                                       :::::::::::" << std::endl << "\
:::::::::::::$$$$$$$$$$  ;;;;  ::  ;;  ::::::::::::  ;;  ::  ;;;;  ::::::::::::" << std::endl << "\
:::::::::::::::::::::::      ::::::    :::::::::::::     ::::      ::::::::::::" << std::endl << "\
:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::" << std::endl << "\
:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::" << std::endl << "\
::::::::::::::::NN::::NN::YY::::YY:::AAAAAA:::NN::::NN:::!!::::::::::::::::::::" << std::endl << "\
::::::::::::::::NNNN::NN::YY::::YY::AA::::AA::NNNN::NN:::!!::::::::::::::::::::" << std::endl << "\
::::::::::::::::NNNN::NN::YY::::YY::AA::::AA::NNNN::NN:::!!::::::::::::::::::::" << std::endl << "\
::::::::::::::::NN::NNNN::::YYYY::::AAAAAAAA::NN::NNNN:::!!::::::::::::::::::::" << std::endl << "\
::::::::::::::::NN::NNNN:::::YY:::::AA::::AA::NN::NNNN:::::::::::::::::::::::::" << std::endl << "\
::::::::::::::::NN::::NN:::::YY:::::AA::::AA::NN::::NN:::!!::::::::::::::::::::" << std::endl << "\
:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::" << std::endl << "\
::::::::::::::::::::::::YOU:HAVE:DONE:THE:NYAN:::::::::::::::::::::::::::::::::" << std::endl;
	std::cout << "Hello there, stranger. Here it is the console. The console which will demonstrate you the wonderful networking possibilities of a basic command-line interface\n";
	std::cout << "Before then, should the console connect you to the localhost network. It is likely that you're not running a production demo of this thing, so console assumes that the localhost would be the right address to find the server.\n";
	std::cout << "Type \"y\" or \"n\"";
	std::cin >> Answer;

	if (Answer == 'y' || Answer == 'Y') {
		std::cout << "Okay then, the console was right, it is just for the local demonstration. \nServer 127.0.0.1 will see this client application in just a moment... \n";
		User.ServerAdress = SERVER_ADDRESS;
	}
	else if (Answer == 'n' || Answer == 'N') {
		char serverAdress[ARRAY_SIZE];
		std::cout << "No? Who said running this in production was a good idea? Specify the IP address to connect in this case?\n";
		std::cin >> serverAdress;
		User.ServerAdress = serverAdress;
	}
	else {
		std::cout << "The console awaits the correct answer. Don't mess with it";
	};

	std::cout << "Console needs to know your first name. It is important: \n";
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
		cout << "You appear to be connected!\n"
			<< "Typing -j means that you're gonna join to the room number ... (which you also have to specify). \n-l means LEAVE!\n";
		system("pause");
	}
	else {
		cout << "Looks like the server is down, no luck this time.\n";
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
			cout << ".-\"-._,-'_`-._,-'_`-._,-'_`-._,-'_`-,_,-'_`-,_,-'_`-,_,-'_`-,_,-'_`-,.\n";
			cout << "The console reminds you! : Typing -j means that you're gonna join to the room number ... (which you also have to specify). \n-l means LEAVE!\n";
			cout << "If the chat isn't updated, use the Enter key, my dear friend!\n";
			cout << "The connection protocol tells about being connected to the next rooms: ";
			for (int i = 0; i < ConnRooms.size(); i++) cout << ConnRooms[i] << " ";
			cout << '\n';
			cout << ".-\"-._,-'_`-._,-'_`-._,-'_`-._,-'_`-,_,-'_`-,_,-'_`-,_,-'_`-,_,-'_`-,.\n";
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