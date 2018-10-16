#include "ConnectionMaintainer.h"

ConnectionMaintainer::ConnectionMaintainer() {}

ConnectionMaintainer::~ConnectionMaintainer() {}

void ConnectionMaintainer::Connect(UserInfo info) {
	// Initialize Winsock
	IResult = WSAStartup(MAKEWORD(2, 2), &winsockData);
	if (IResult != 0) {
		printf("WSAStartup Error: %d\n", IResult);
		return;
	}

	ZeroMemory(&Hints, sizeof(Hints));
	Hints.ai_family = AF_UNSPEC;
	Hints.ai_socktype = SOCK_STREAM;
	Hints.ai_protocol = IPPROTO_TCP;

	IResult = getaddrinfo(info.ServerAdress, PORT_NUMBER, &Hints, &InfoResult);
	if (IResult != 0) {
		printf("getaddrinfo() Error: %d\n", IResult);
		WSACleanup();
		return;
	}

	// Connecting to the adress
	for (Pointer = InfoResult; Pointer != NULL; Pointer = Pointer->ai_next) {
		// Create a socket to connect
		ConnectionSocket = socket(Pointer->ai_family, Pointer->ai_socktype, Pointer->ai_protocol);
		if (ConnectionSocket == INVALID_SOCKET) {
			printf("socket() Error: %d\n", IResult);
			WSACleanup();
			return;
		}

		// Connecting to server
		IResult = connect(ConnectionSocket, Pointer->ai_addr, (int)Pointer->ai_addrlen);
		if (IResult == SOCKET_ERROR) {
			closesocket(ConnectionSocket);
			ConnectionSocket = INVALID_SOCKET;
			continue;
		}

		break;
	}

	freeaddrinfo(InfoResult);

	if (ConnectionSocket == INVALID_SOCKET) {
		printf("Unable to connect to server, check the adress \n");
		WSACleanup();
		return;
	}

	// success
	this->isAlive = 1;
}


void ConnectionMaintainer::closeConnection() {
	IResult = shutdown(ConnectionSocket, SD_SEND);
	if (IResult == SOCKET_ERROR) {
		printf("shutdown() error %d\n", IResult);
		closesocket(ConnectionSocket);
		WSACleanup();
		return;
	}

	closesocket(ConnectionSocket);
	WSACleanup();
}

void ConnectionMaintainer::sendMessage(Buffer* connBuff, UserInfo info, char msgID,
	string message) {
	int packetLenght = 0;

	switch (msgID) {
	case MSG_ID_JOIN_ROOM: {  // Block it for local variables

	  // Name lenght:
		short userNameLenght = strlen(info.Name);

		// Room number lenght:
		short roomNumberNameLenght = strlen(info.Room);

		packetLenght = sizeof(int) + sizeof(char) + sizeof(short) +
			roomNumberNameLenght + sizeof(short) + userNameLenght;

		// Writing
		connBuff->WriteInt32LE(packetLenght);
		connBuff->WriteChar(MSG_ID_JOIN_ROOM);

		connBuff->WriteInt16LE(roomNumberNameLenght);
		for (int i = 0; i < roomNumberNameLenght; i++)
			connBuff->WriteChar(info.Room[i]);

		connBuff->WriteInt16LE(userNameLenght);
		for (int i = 0; i < userNameLenght; i++)
			connBuff->WriteChar(info.Name[i]);

	}

						   break;

	case MSG_ID_LEAVE_ROOM: {
		// Room number lenght:
		short roomNumberNameLenght = strlen(info.Room);

		packetLenght =
			sizeof(int) + sizeof(char) + sizeof(short) + roomNumberNameLenght;

		// Writing
		connBuff->WriteInt32LE(packetLenght);
		connBuff->WriteChar(MSG_ID_LEAVE_ROOM);

		connBuff->WriteInt16LE(roomNumberNameLenght);
		for (int i = 0; i < roomNumberNameLenght; i++)
			connBuff->WriteChar(info.Room[i]);

	}

							break;

	case MSG_ID_LEAVE_THE_MESSAGE: {
		// Message lenght
		short msgLenght = message.size();

		packetLenght = sizeof(int) + sizeof(char) + sizeof(short) + msgLenght;

		// Writing
		connBuff->WriteInt32LE(packetLenght);
		connBuff->WriteChar(MSG_ID_LEAVE_THE_MESSAGE);

		connBuff->WriteInt16LE(msgLenght);
		for (int i = 0; i < msgLenght; i++) connBuff->WriteChar(message.at(i));

	}

								   break;

	default:
		break;
	}

	// Send Messege
	IResult = send(ConnectionSocket, connBuff->getBuffer(), packetLenght, 0);
	if (IResult == SOCKET_ERROR) {
		printf("Socket() Error: %d\n", IResult);
		closesocket(ConnectionSocket);
		WSACleanup();
		return;
	}
	else {
		connBuff->clearBuffer();
	}
}