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