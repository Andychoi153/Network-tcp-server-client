//
// Server.cpp
//
// Extremely simple, stream server example.
// Works in conjunction with Client.cpp.
//
// The program sets itself up as a server using the TCP
// protoocl. It waits for data from a client, displays
// the incoming data, sends a message back to the client
// and then exits.
//
// Compile and link with wsock32.lib
//
// Pass the port number that the server should bind() to
// on the command line. Any port number not already in use
// can be specified.
//
// Example: Server 2000
//


#include <stdio.h>
#include "Winsock2.h"
#include <string.h>
#include <stdlib.h>

// Function prototype
void StreamServer(short nPort);

// Helper macro for displaying errors
#define PRINTERROR(s)	\
	fprintf(stderr, "\n%: %d\n", s, WSAGetLastError())

// Link with Ws2_32.lib required
#pragma comment(lib, "Ws2_32.lib")

void main(int argc, char **argv)
{
	WORD wVersionRequested = MAKEWORD(2, 2);
	WSADATA wsaData;
	int nRet; 	short nPort;

	// Check for port argument
	if (argc == 2)
	{
		nPort = atoi(argv[1]);
	}
	else if (argc == 1)
	{
		fprintf(stdout, "Please enter port number: ");
		scanf_s("%hu", &nPort, sizeof(nPort));
	}
	else
	{
		fprintf(stderr, "\nSyntax: server PortNumber\n");
		return;
	}


	// Initialize WinSock and check version
	nRet = WSAStartup(wVersionRequested, &wsaData);
	if (wsaData.wVersion != wVersionRequested)
	{
		fprintf(stderr, "\n Wrong version\n");
		return;
	}

	// Do the stuff a stream server does
	StreamServer(nPort);

	// Release WinSock
	WSACleanup();
}
void StreamServer(short nPort)
{
	//
	// Create a TCP/IP stream socket to "listen" with
	//
	SOCKET	listenSocket;

	listenSocket = socket(AF_INET,	// Address family
		SOCK_STREAM,	// Socket type
		IPPROTO_TCP);	// Protocol
	if (listenSocket == INVALID_SOCKET)
	{
		PRINTERROR("socket()");
		return;
	}

	// Fill in the address structure
	SOCKADDR_IN saServer;

	saServer.sin_family = AF_INET;
	saServer.sin_addr.s_addr = htonl(INADDR_ANY);
	// .sin_addr.S_un.S_addr = htonl(INADDR_ANY);
	// Let WinSock supply address
	saServer.sin_port = htons(nPort);
	// Use port from command line

	// bind the name to the socket
	int nRet;

	nRet = bind(listenSocket,		// Socket 
		(LPSOCKADDR)&saServer,	// Our address
		sizeof(struct sockaddr));	// Size

	if (nRet == SOCKET_ERROR)
	{
		PRINTERROR("bind()");
		closesocket(listenSocket);
		return;
	}


	// This isn't normally done or required, but in this 
	// example we're printing out where the server is 
	// waiting  so that you can connect the example 
	// client.
	int nLen;
	nLen = sizeof(SOCKADDR);
	char szBuf[256];

	nRet = gethostname(szBuf, sizeof(szBuf));
	if (nRet == SOCKET_ERROR)
	{
		PRINTERROR("gethostname()");
		closesocket(listenSocket);
		return;
	}

	// Show the server name and port number
	printf("\nServer named %s waiting on port %d\n",
		szBuf, nPort);

	// Set the socket to listen

	printf("\nlisten()");
	nRet = listen(listenSocket,	// Bound socket
		SOMAXCONN);
	// Number of connection request queue
	if (nRet == SOCKET_ERROR)
	{
		PRINTERROR("listen()");
		closesocket(listenSocket);
		return;
	}






	// Wait for an incoming request
	SOCKET	remoteSocket;

	printf("\nBlocking at accept()");
	remoteSocket = accept(listenSocket,
		// Listening socket
		NULL,
		// Optional client address
		NULL);
	if (remoteSocket == INVALID_SOCKET)
	{
		PRINTERROR("accept()");
		closesocket(listenSocket);
		return;
	}
	int k =0;
	char Data[256];
	// We're connected to a client
	// New socket descriptor returned already
	// has clients address
	int i = 0;
	int j = 0;
	while (1) {
		char client[10];
		char *pclient;
		
		//여기서 부터
		// Recedive data from the client
		memset(Data, 0, sizeof(Data));
		nRet = recv(remoteSocket,	// Connected client
			Data,			// Receive buffer
			sizeof(Data),	// Lenght of buffer
			0);				// Flags
		if (nRet == INVALID_SOCKET)
		{
			PRINTERROR("recv()");
			closesocket(listenSocket);
			closesocket(remoteSocket);
			return;
		}
		
		// Display received data
		//if (i == 1) {
			if (i==1&&((strstr(Data, "ADD") != NULL) || (strstr(Data, "ADd")!=NULL) || (strstr(Data, "Add") != NULL) || (strstr(Data, "add") != NULL) || (strstr(Data, "aDD") != NULL) || (strstr(Data, "aDd") != NULL) || (strstr(Data, "adD") != NULL))) {
				i == 2;
				if (strstr(Data, " ") != NULL) {
					pclient = strstr(Data, " ");
					sprintf_s(client, sizeof(client), "%s", pclient + 8);

					memset(Data, 0, sizeof(Data));
					sprintf_s(Data, sizeof(Data), "200 Welcome %s. What can I do for you?", client);
					i = 1;
					printf("you got mail\n");
					strcpy_s(Data, sizeof(Data), "you write ADD");
				}
				//else
			}
			


		//printf("\nData received: %s 원본", szBuf);

		else if (i==0&&(strstr(Data, "Hello") != NULL)) {

			//printf("\nData received: %s", szBuf);
			pclient = strstr(Data, "This is");
			sprintf_s(client, sizeof(client), "%s", pclient + 8);

			memset(Data, 0, sizeof(Data));
			sprintf_s(Data, sizeof(Data), "200 Welcome %s. What can I do for you?", client);
			i = 1;
			j = 0;
		}

		

		else if (strstr(Data, "QUIT") != NULL) {
			strcpy_s(Data, sizeof(Data), "Thanks. Have a nice day!");
			nRet = send(remoteSocket,	// Connected socket
				Data,			// Data buffer
				strlen(Data),	// Lenght of data
				0);				// Flags
								//여개까지 while 문 처리할것
			memset(Data, 0, sizeof(Data));
			closesocket(remoteSocket);
			closesocket(listenSocket);
			return;
		}

		else {
			if (i == 0) {
				printf("\nData received: %s", Data);
				memset(Data, 0, sizeof(Data));
				strcpy_s(Data, sizeof(Data), "Greeting Fisrt");
			}
			if (i==1){ 
				printf("\nData received: %s", Data);
				memset(Data, 0, sizeof(Data));
				strcpy_s(Data, sizeof(Data), "401 Unknown command");

			}
		}

		//memset(szBuf, 0, sizeof(szBuf));

		// Send data back to the client
		//strcpy_s(szBuf, sizeof(szBuf), "From the hello");
		nRet = send(remoteSocket,	// Connected socket
			Data,			// Data buffer
			strlen(Data),	// Lenght of data
			0);				// Flags
		//여개까지 while 문 처리할것
		memset(Data, 0, sizeof(Data));


		// Close BOTH sockets before exiting
		
	}
}

