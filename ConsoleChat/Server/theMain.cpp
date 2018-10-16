#define WIN32_LEAN_AND_MEAN

#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <vector>

#include "Buffer.h"

// Need to link with Ws2_32.lib
#pragma comment(lib, "Ws2_32.lib")

// #pragma comment (lib, "Mswsock.lib")
#define BUFFER_LENGTH 512  // Default buffer length of our buffer in characters
#define PORT_NUMBER "5000"  // The default port to listen on

typedef struct _SOCKET_INFORMATION {
	char Buffer[BUFFER_LENGTH];
	WSABUF WsaBuffer;
	SOCKET Socket;
	OVERLAPPED Overlapped;
	DWORD BytesSEND;
	DWORD BytesRECV;
	std::vector<std::string> rooms;  // Rooms
	std::string UserName;            // Name
	bool GotNewData = 0;
} SOCKET_INFORMATION, *LPSOCKET_INFORMATION;

LPSOCKET_INFORMATION SocketArray[FD_SETSIZE];
int iRes;
FD_SET WriteSet;
FD_SET ReadSet;
BOOL CreateSocketInformation(SOCKET s);
void FreeSocketInformation(DWORD Index);
void ReadSocket(LPSOCKET_INFORMATION sa);
void sendMsg(LPSOCKET_INFORMATION sa, std::string msg, std::string userName);
void TreatMessage(LPSOCKET_INFORMATION sa, std::string msg);
DWORD TotalSockets = 0;

int main(void)
{
	//Winsock
	WSADATA winsockData;
	SOCKET clientSocket;
	SOCKET listenSocket;
	SOCKADDR_IN InternetAddress;
	DWORD i, Total;
	ULONG NonBlock;
	DWORD Flags, SendBytes, RecvBytes;

	//address
	struct addrinfo* result = NULL; 
	struct addrinfo hints;
	ZeroMemory(&hints, sizeof(hints));

	// Initialize Winsock
	iRes = WSAStartup(MAKEWORD(2, 2), &winsockData);
	if (iRes != 0)
	{
		printf("WSAStart Fail: %d\n", iRes);
		return 1;
	}

	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;
	hints.ai_flags = AI_PASSIVE;

	listenSocket = socket(AF_INET, SOCK_STREAM, 0);
	if (listenSocket == INVALID_SOCKET) 
	{
		printf(":: Called Function :: socket() | INFO :: Error: %d\n", WSAGetLastError());
		WSACleanup();
		return 1;
	}

	getaddrinfo(NULL, PORT_NUMBER, &hints, &result);
	iRes = bind(listenSocket, result->ai_addr, (int)result->ai_addrlen);
	if (iRes == SOCKET_ERROR)
	{
		printf(":: Called Function :: bind() | INFO :: failed with error: %d\n", WSAGetLastError());
		freeaddrinfo(result);
		closesocket(listenSocket);
		WSACleanup();
		return 1;
	}

	if (listen(listenSocket, SOMAXCONN))
	{
		printf(":: Called Function :: listen() | INFO :: Error: %d\n", WSAGetLastError());
		closesocket(listenSocket);
		WSACleanup();
		return 1;
	}
	
	NonBlock = 1;
	if (ioctlsocket(listenSocket, FIONBIO, &NonBlock) == SOCKET_ERROR)
	{
		printf(":: Called Function :: ioctlsocket() | INFO :: Error: %d\n", WSAGetLastError());
		return 1;
	}
	else
	{
		printf(":: Called Function :: ioctlsocket() | INFO :: OK!\n");
	}


	while (true) 
	{
		FD_ZERO(&ReadSet);
		FD_ZERO(&WriteSet);

		FD_SET(listenSocket, &ReadSet);

		for (i = 0; i < TotalSockets; i++)
		{
			if (SocketArray[i]->GotNewData)
			{
				FD_SET(SocketArray[i]->Socket, &WriteSet);
			}
			else
			{
				FD_SET(SocketArray[i]->Socket, &ReadSet);
			}
		}

		if ((Total = select(0, &ReadSet, &WriteSet, NULL, NULL)) == SOCKET_ERROR)
		{
			printf(":: Called Function :: select() | INFO :: Error: %d\n", WSAGetLastError());
			return 1;
		}
		else
		{
			printf(":: Called Function :: select() | INFO :: OK!\n");
		}

		if (FD_ISSET(listenSocket, &ReadSet))
		{
			Total--;
			if ((clientSocket = accept(listenSocket, NULL, NULL)) != INVALID_SOCKET)
			{
				NonBlock = 1;
				if (ioctlsocket(clientSocket, FIONBIO, &NonBlock) == SOCKET_ERROR)
				{
					printf(":: Called Function :: ioctlsocket(FIONBIO | INFO ::) Error: %d\n", WSAGetLastError());
					return 1;
				}
				else
				{
					printf(":: Called Function :: ioctlsocket(FIONBIO | INFO ::) OK!\n");
				}

				if (CreateSocketInformation(clientSocket) == FALSE)
				{
					printf(":: Called Function :: CreateSocketInformation(clientSocket | INFO ::) failed!\n");
					return 1;
				}
				else
				{
					printf(":: Called Function :: CreateSocketInformation() | INFO :: OK!\n");
				}
			}
			else
			{
				if (WSAGetLastError() != WSAEWOULDBLOCK)
				{
					printf(":: Called Function :: accept() | INFO :: Error: %d\n", WSAGetLastError());
					return 1;
				}
				else
				{
					printf(":: Called Function :: accept() | INFO :: OK!\n");
				}
			}
		}

		for (i = 0; Total > 0 && i < TotalSockets; i++)
		{
			LPSOCKET_INFORMATION SocketInfo = SocketArray[i];

			if (FD_ISSET(SocketInfo->Socket, &ReadSet))
			{
				Total--;

				SocketInfo->WsaBuffer.buf = SocketInfo->Buffer;
				SocketInfo->WsaBuffer.len = BUFFER_LENGTH;

				Flags = 0;
				if (WSARecv(SocketInfo->Socket, &(SocketInfo->WsaBuffer), 1, &RecvBytes, &Flags, NULL, NULL) == SOCKET_ERROR)
				{
					if (WSAGetLastError() != WSAEWOULDBLOCK) 
					{
						printf("WSARecv() Error: %d\n", WSAGetLastError());
						FreeSocketInformation(i);
					}
					else 
					{
						printf("WSARecv() OK!\n");
					}
					continue;
				}
				else
				{
					SocketInfo->BytesRECV = RecvBytes;

					if (RecvBytes == 0) 
					{
						FreeSocketInformation(i);
						continue;
					}
					else
					{
						ReadSocket(SocketArray[i]);
					}
				}
			}

			if (FD_ISSET(SocketInfo->Socket, &WriteSet))
			{
				Total--;

				if (SocketInfo->GotNewData)
				{
					if (WSASend(SocketInfo->Socket, &(SocketInfo->WsaBuffer), 1, &SendBytes, 0, NULL, NULL) == SOCKET_ERROR)
					{
						if (WSAGetLastError() != WSAEWOULDBLOCK) 
						{
							printf("WSASend() failed with error %d\n", WSAGetLastError());
							FreeSocketInformation(i);
						}
						else
						{
							printf("WSASend() OK!\n");
						}
						continue;
					}
					else
					{
						SocketInfo->BytesSEND += SendBytes;

						if (SocketInfo->BytesSEND == SocketInfo->BytesRECV)
						{
							SocketInfo->BytesSEND = 0;
							SocketInfo->BytesRECV = 0;
						}
					}
				}
				SocketInfo->GotNewData = 0;
			}
		}
	}
}