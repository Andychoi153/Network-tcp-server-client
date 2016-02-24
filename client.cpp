//
// Client.cpp
//
// Extremely simple, stream client example.
// Works in conjunction with Server.cpp.
//
// The program attempts to connect to the server and port
// specified on the command line. The Server program prints
// the needed information when it is started. Once connected,
// the program sends data to the server, waits for a response
// and then exits.
//
// Compile and link with wsock32.lib
//
// Pass the server name and port number on the command line. 
//
// Example: Client MyMachineName 2000
//

#include <stdio.h>
#include "Winsock2.h"

// Function prototype
void StreamClient(char *szServer, short nPort);

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
	char addr[15] = { '\0', };

	// Check for the host and port arguments
	if (argc == 3)
	{
		nPort = atoi(argv[2]);
		strcpy_s(addr, sizeof(addr), argv[1]);
	}
	else if (argc == 1)
	{
		fprintf(stdout, "\nInput server address : ");
		gets_s(addr, sizeof(addr));
		fprintf(stdout, "Input server port number : ");
		scanf_s("%hu", &nPort, sizeof(nPort));
	}
	else
	{
		fprintf(stderr, "\nSyntax: client ServerName PortNumber\n");
		return;
	}

	// Initialize WinSock and check the version
	nRet = WSAStartup(wVersionRequested, &wsaData);
	if (wsaData.wVersion != wVersionRequested)
	{
		fprintf(stderr, "\n Wrong version\n");
		return;
	}

	// Go do the stuff a stream client does
	StreamClient(addr, nPort);

	// Release WinSock
	WSACleanup();
}
void StreamClient(char *szServer, short nPort)
{
	printf("\nStream Client connecting to server: %s on port: %d\n", szServer, nPort);

	// Find the server
	LPHOSTENT lpHostEntry;
	// HOSTENT *HostEntry;

	lpHostEntry = gethostbyname(szServer);
	if (lpHostEntry == NULL)
	{
		PRINTERROR("gethostbyname()");
		return;
	}

	// Create a TCP/IP stream socket
	SOCKET	theSocket;

	theSocket = socket(AF_INET,  //Address family
		SOCK_STREAM,	 // Socket type
		IPPROTO_TCP); // Protocol
	if (theSocket == INVALID_SOCKET)
	{
		PRINTERROR("socket()");
		return;
	}

	// Fill in the address structure
	SOCKADDR_IN saServer;

	saServer.sin_family = AF_INET;
	saServer.sin_addr =
		*((LPIN_ADDR)*lpHostEntry->h_addr_list);
	// Server's address

	saServer.sin_port = htons(nPort);
	// Port number from command line

	// connect to the server
	int nRet;

	nRet = connect(theSocket,	   // Socket
		(LPSOCKADDR)&saServer,	// Server address
		sizeof(struct sockaddr)); // Length 
	if (nRet == SOCKET_ERROR)
	{
		PRINTERROR("socket()");
		closesocket(theSocket);
		return;
	}

	// Send data to the server
	int k = 0;
	char szBuf[256];
	//여기서 부터
	while (1) {
		
		
		memset(szBuf, 0, sizeof(szBuf));

		//scanf_s("%[^.]s", szBuf, sizeof(szBuf));
		if(k == 0){ scanf_s("%[^.]s", szBuf, sizeof(szBuf)); }
		//if (k == 1) { scanf_s("%20s", szBuf, sizeof(szBuf)); }
		if (k == 1) { scanf_s("%20[^#]s", szBuf, sizeof(szBuf)); }
		nRet = send(theSocket,	     // Connected socket
			szBuf,	          // Data buffer
			strlen(szBuf),	// Length of data
			0);				// Flags

		if (nRet == SOCKET_ERROR)
		{
			PRINTERROR("send()");
			closesocket(theSocket);
			return;
			break;
		}
		memset(szBuf, 0, sizeof(szBuf));
		// Wait for a reply
		nRet = recv(theSocket,		// Connected socket
			szBuf,			// Receive buffer
			sizeof(szBuf),	// receive buffer size
			0);				// Flags
		if (nRet == SOCKET_ERROR)
		{
			PRINTERROR("recv()");
			closesocket(theSocket);
			return;
			break;
		}
		// Display the received data
		printf("\nData received: %s\n", szBuf);
		printf("hello");
		if (strstr(szBuf, "Thanks") != NULL) {
			memset(szBuf, 0, sizeof(szBuf));
			closesocket(theSocket);
			return;
		}//끝나면 나가는 구문
		if (strstr(szBuf, "Welcome") != NULL) { k = 1; }
	}
	//여기까지 while 구문


}

