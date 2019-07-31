#pragma once
#include <iostream>
#include <vector>
#include <WinSock2.h>
#include <WS2tcpip.h>
#pragma comment(lib,"Ws2_32.lib")

using namespace std;

#define PORT 27016
#define IP "127.0.0.1"
class server
{
private:
	WSADATA wsaData;
	SOCKADDR_IN serverInfo, clientInfo;
	int sockLen = sizeof(clientInfo);
	vector<SOCKET> socketList;
	SOCKET listenSocket = INVALID_SOCKET, clientSocket =  INVALID_SOCKET ;
public:
	server(const char *ip,int port);
	~server();
	void ServerStart();
	int Accpte();
	int SendAll(char *sendData,int size);
	int DataRecv(SOCKET s, char *recvData, int size);
	int DataSend(SOCKET s, char *recvData, int size);
	void GetClientIP(char* getip);
	SOCKET GetClientSocket();
	void RemoveClient(SOCKET s);
	void Error(int error);
	void Close();
};