#pragma once
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <stdexcept>

#pragma comment(lib,"Ws2_32.lib")

typedef struct clientList
{
	SOCKET s;
	SOCKADDR_IN client;
	char Address[INET_ADDRSTRLEN];
}CLIENTINFO;

class NetworkBase
{
protected:
	int errorCode;
	WSADATA wsaData;
	SOCKADDR_IN baseInfo;
	SOCKET baseSocket = INVALID_SOCKET;
public:
	NetworkBase();
	~NetworkBase();
    int Recv(char* Data, int Size);
    int Send(char* Data, int Size);
	void Error(int code, char** msgbuf);
	void Close();
};

