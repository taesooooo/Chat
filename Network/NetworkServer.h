#pragma once
#include <vector>
#include <thread>
#include "NetworkBase.h"


class NetworkServer :public NetworkBase
{
private:
	bool serverState = false;
	SOCKADDR_IN clientInfo;
	std::vector<CLIENTINFO> clientList;
public:
	NetworkServer();
	~NetworkServer();
	int Start(const char* ip,int port);
	int Accept();
	int Recv(int s, char* recvData,int len);
	void RecvAll(char* recvData, int size);
	void SendAll(char* sendData, int size);
	void RemoveClient();
	void RemoveClient(int index);
	char* GetClientIP();
	int GetClientCount();
	bool GetServerState();
	void Close();
};

