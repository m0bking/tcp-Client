#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <WS2tcpip.h>
#include <winsock2.h>

#pragma comment (lib, "ws2_32.lib")

/*CLIENT SERVER*/

void main() {
	
		//IP//
	char IPAdress[15] = "172.16.100.20";
	

		//Listening port # of the server//
	int port = 60000;
	

		//WINSOCK//
	WSADATA data;
	WORD version = MAKEWORD(2, 2);
	int wsResult = WSAStartup(version, &data);
	if (wsResult != 0) 
	{
		printf("Can't start Winsock, Error #%d\n", wsResult);
		return;
	}


		// Socket() // AF-AddressFamily //
	SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock == INVALID_SOCKET)
	{
		printf( "Can't create socket, Error #%d\n", WSAGetLastError);
		WSACleanup();
		return;
	}


		// hint structure ??? >> which server & which port //
	SOCKADDR_IN hint;
	hint.sin_family = AF_INET;
	hint.sin_port = htons(port);	//host to network//
	inet_pton(AF_INET, IPAdress, &hint.sin_addr);
	

		// connect() to server //
	int Connection = connect(sock, (SOCKADDR*)&hint, sizeof(hint));
	if (Connection == SOCKET_ERROR)
	{
		printf("Can't connect to server, Error #%d\n", WSAGetLastError);
		closesocket(sock);
		WSACleanup();
		return;
	}


		// Do-While Loops to send & recv data //
	char buff[4096];
	char UserInput[256];

	do {
		ZeroMemory(buff, 4096);
		int bytesRecv = recv(sock, buff, 4096, 0);
		if (bytesRecv > 0)
		{
			//echo//
			printf("Server: %s\n", buff);
		}
		if (_kbhit) {
			ZeroMemory(UserInput, 256);
			gets(UserInput);

			if (sizeof(UserInput) > 0)
			{
				//send text//
				int sendResult = send(sock, UserInput, sizeof(UserInput), 0);

				if (sendResult != SOCKET_ERROR)
				{
					//wait for response//
					ZeroMemory(buff, 4096);
					int bytesRecv = recv(sock, buff, 4096, 0);
					if (bytesRecv > 0)
					{
						//echo//
						printf("Server: %s\n", buff);
					}
				}
			}
		}
	} while (sizeof(UserInput) > 0);

	closesocket(sock);
	WSACleanup();
}