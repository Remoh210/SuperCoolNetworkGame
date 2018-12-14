#define WIN32_LEAN_AND_MEAN

#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <vector>

#include "../cMeshObject.h"
#include "Buffer.h"

// Need to link with Ws2_32.lib
#pragma comment(lib, "Ws2_32.lib")

// #pragma comment (lib, "Mswsock.lib")
#define BUFFER_LENGTH 512  // Default buffer length of our buffer in characters
#define PORT_NUMBER "5000"  // The default port to listen on

int userNum = 0;

typedef struct _SOCKET_INFORMATION {
	char Buffer[BUFFER_LENGTH];
	WSABUF WsaBuffer;
	SOCKET Socket;
	OVERLAPPED Overlapped;
	DWORD BytesSEND;
	DWORD BytesRECV;
	std::vector<std::string> rooms;  // Rooms
	std::string UserName;            // Name
	cMeshObject * obj;
	bool GotNewData = 0;
	int id;
} SOCKET_INFORMATION, *LPSOCKET_INFORMATION;

LPSOCKET_INFORMATION SocketArray[FD_SETSIZE];
int iRes;
FD_SET WriteSet;
FD_SET ReadSet;
BOOL CreateSocketInformation(SOCKET s);
void FreeSocketInformation(DWORD Index);
void ReadSocket(LPSOCKET_INFORMATION sa);
void sendMsg(LPSOCKET_INFORMATION sa, short id, std::string msg, std::string userName);
void TreatMessage(LPSOCKET_INFORMATION sa, std::string msg);
DWORD TotalSockets = 0;

float yPos = -0.2f;
float xPos = -1.5f;
float zPos = -74.0f;

float yRotation = 0.0f;

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

BOOL CreateSocketInformation(SOCKET s)
{
	LPSOCKET_INFORMATION SI;
	printf("SOCKET # %d\n :: Accepted", s);

	if ((SI = (LPSOCKET_INFORMATION)GlobalAlloc(GPTR, sizeof(SOCKET_INFORMATION))) == NULL) 
	{
		printf(":: Called Function :: GlobalAlloc() | INFO :: Erro %d\n", GetLastError());
		return FALSE;
	}
	else
	{
		printf(":: Called Function :: GlobalAlloc() | INFO :: for SOCKET_INFORMATION is OK!\n");
	}

	SI->Socket = s;
	SI->BytesSEND = 0;
	SI->BytesRECV = 0;

	SocketArray[TotalSockets] = SI;
	TotalSockets++;
	return (TRUE);
}

void FreeSocketInformation(DWORD Index) 
{
	LPSOCKET_INFORMATION SI = SocketArray[Index];
	DWORD i;

	closesocket(SI->Socket);
	printf("Closing socket number %d\n", SI->Socket);
	GlobalFree(SI);

	for (i = Index; i < TotalSockets; i++)
	{
		SocketArray[i] = SocketArray[i + 1];
	}

	TotalSockets--;
}

void ReadSocket(LPSOCKET_INFORMATION sa)
{
	bool isMessageIncomplete = 1;
	int currMsgLength = 0;
	std::string currBuffer;

	for (int i = 0; i <= sa->BytesRECV; i++)
	{
		if (isMessageIncomplete)
		{
			if (!currMsgLength) 
			{
				currBuffer.push_back(sa->WsaBuffer.buf[i]);

				if (currBuffer.size() == 4)
				{
					Buffer buff(4);
					for (int index2 = 0; index2 < 4; index2++)
					{
						buff.WriteChar(currBuffer.at(index2));
					}
					currMsgLength = buff.ReadInt32LE();
				}
			}
			else
			{
				if (currMsgLength != currBuffer.size())
				{
					currBuffer.push_back(sa->WsaBuffer.buf[i]);
				}
				else
				{
					TreatMessage(sa, currBuffer);
					currBuffer = "";
					!isMessageIncomplete;
					!currMsgLength;
				}
			}
		}
		else
		{
			currMsgLength = 0;
			!isMessageIncomplete;
			currBuffer.push_back(sa->WsaBuffer.buf[i]);
		}
	}
}

void sendMsg(LPSOCKET_INFORMATION sa, short id, std::string msg, std::string userName)
{
	std::string formatedMsg = userName + "->" + msg;

	int packetLength = sizeof(INT32) + sizeof(short) + formatedMsg.size();

	Buffer buff(packetLength);
	buff.WriteInt32LE(packetLength);

	buff.WriteInt16LE(id);

	for (int index2 = 0; index2 < formatedMsg.size(); index2++)
	{
		buff.WriteChar(formatedMsg.at(index2));
	}

	buff.WriteChar('\0');

	sa->WsaBuffer.buf = buff.getBuffer();
	sa->WsaBuffer.len = packetLength;
	sa->GotNewData = 1;
}
void sendMsgValue(LPSOCKET_INFORMATION sa, int id, std::string msg, std::string userName)
{
	std::string formatedMsg = msg;

	int packetLength = sizeof(INT32) + sizeof(int) + formatedMsg.size();

	Buffer buff(packetLength);
	buff.WriteInt32LE(packetLength);
	buff.WriteInt16LE(id);

	for (int index2 = 0; index2 < formatedMsg.size(); index2++)
	{
		buff.WriteChar(formatedMsg.at(index2));
	}

	buff.WriteChar('\0');

	sa->WsaBuffer.buf = buff.getBuffer();
	sa->WsaBuffer.len = packetLength;
	sa->GotNewData = 1;
}
#include <iostream>
void TreatMessage(LPSOCKET_INFORMATION sa, std::string msg)
{
	Buffer buff(msg.size());

	for (int index2 = 0; index2 < msg.size(); index2++)
	{
		buff.WriteChar(msg.at(index2));
	}

	int packetLenght = buff.ReadInt32LE();

	char msgID = buff.ReadChar();

	switch (msgID)
	{
		case 1:
		{
			short roomNameLenght = buff.ReadInt16LE();
			std::string roomName;

			for (short index3 = 0; index3 < roomNameLenght; index3++)
			{
				roomName.push_back(buff.ReadChar());
			}

			sa->rooms.push_back(roomName);
			short userNameLenght = buff.ReadInt16LE();
			std::string userName;

			for (short index3 = 0; index3 < userNameLenght; index3++)
			{
				userName.push_back(buff.ReadChar());
			}
			sa->UserName = userName + " ";
			sa->id = userNum; //assign this user/socket an id
			userNum++; //increment the next id to be given

			for (int indA = 0; indA < sa->rooms.size(); indA++)
			{
				for (int indB = 0; indB < TotalSockets; indB++)
				{
					for (int indC = 0; indC < SocketArray[indB]->rooms.size(); indC++)
					{
						if (roomName == SocketArray[indB]->rooms.at(indC) && SocketArray[indB]->rooms.at(indC) != "")
						{
							sendMsg(SocketArray[indB], 0, "JoinAs" + std::to_string(sa->id), "Server");
							//sendMsg(SocketArray[indB], 0, userName + " has connected to " + roomName, "Server");
						}
					}
				}
			}

			sa->obj = new cMeshObject();
			sa->obj->position = glm::vec3(xPos, yPos, zPos);

		}break;

		case 2:
		{
			short roomNameLenght = buff.ReadInt16LE();
			std::string roomName;

			for (short index3 = 0; index3 < roomNameLenght; index3++)
			{
				roomName.push_back(buff.ReadChar());
			}

			for (int indA = 0; indA < sa->rooms.size(); indA++)
			{
				if (roomName == sa->rooms[indA])
				{
					sa->rooms[indA].erase();

					for (int indB = 0; indB < sa->rooms.size(); indB++)
					{
						for (int indC = 0; indC < TotalSockets; indC++)
						{
							for (int indD = 0; indD < SocketArray[indC]->rooms.size(); indD++)
							{
								if (roomName == SocketArray[indC]->rooms.at(indD) && SocketArray[indC]->rooms.at(indD) != "")
								{
									sendMsg(SocketArray[indC], 0,	sa->UserName + " has disconnected from " + roomName, "Server");
								}
							}
						}
					}
				}
			}
		}break;

		// MSG_ID_LEAVE_THE_MESSAGE
		case 3:
		{
			short msgLenght = buff.ReadInt16LE();
			std::string msg;
			for (short index3 = 0; index3 < msgLenght; index3++)
			{
				msg.push_back(buff.ReadChar());
			}

			for (int indA = 0; indA < sa->rooms.size(); indA++)
			{
				for (int indB = 0; indB < TotalSockets; indB++)
				{
					for (int indC = 0; indC < SocketArray[indB]->rooms.size(); indC++)
					{
						if (sa->rooms.at(indA) == SocketArray[indB]->rooms.at(indC) && sa->rooms.at(indA) != "")
						{
							sendMsg(SocketArray[indB], 0, msg, sa->UserName);
						}
					}
				}
			}
		}break;
		// update z pos
		case 4:
		{
			short id = buff.ReadInt16LE();
			bool forceCSP = false;
			// force CSP
			if (forceCSP) {
				id++;
			}
			short msgLenght = buff.ReadInt16LE();
			std::string msg;
			for (short index3 = 0; index3 < msgLenght; index3++)
			{
				msg.push_back(buff.ReadChar());
			}
			std::string buildstring = "";
			for (unsigned int index = 0; index != msg.size(); index++)
			{
					std::cout << msg[index] << std::endl;
					buildstring += std::to_string(msg[index]);
			}
			
			for (int indA = 0; indA < sa->rooms.size(); indA++)
			{
				for (int indB = 0; indB < TotalSockets; indB++)
				{
					for (int indC = 0; indC < SocketArray[indB]->rooms.size(); indC++)
					{
						if (sa->rooms.at(indA) == SocketArray[indB]->rooms.at(indC) && sa->rooms.at(indA) != "")
						{
							sa->obj->position.z += 0.1f;
							//zPos += 0.1f;
							// id+ zpos
							//std::string sendthis = buildstring + ":" + std::to_string(zPos);
							// zpos
							std::string sendthis = std::to_string(sa->obj->position.z);
							sendMsgValue(SocketArray[indB], id, sendthis, sa->UserName);
						}
					}
				}
			}
		}break;

		// Y rotation input
		case 5:
		{
			short id = buff.ReadInt16LE();
			short msgLenght = buff.ReadInt16LE();
			std::string msg;
			for (short index3 = 0; index3 < msgLenght; index3++)
			{
				msg.push_back(buff.ReadChar());
			}

			for (int indA = 0; indA < sa->rooms.size(); indA++)
			{
				for (int indB = 0; indB < TotalSockets; indB++)
				{
					for (int indC = 0; indC < SocketArray[indB]->rooms.size(); indC++)
					{
						if (sa->rooms.at(indA) == SocketArray[indB]->rooms.at(indC) && sa->rooms.at(indA) != "")
						{
							yRotation += 0.1f;
							std::string sendthis = std::to_string(zPos);
							sendMsgValue(SocketArray[indB], id, sendthis, sa->UserName);
						}
					}
				}
			}
		}break;

		default:
			break;
	}
}