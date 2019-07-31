#include "NetworkServer.h"



NetworkServer::NetworkServer()
{

}


NetworkServer::~NetworkServer()
{

}

int NetworkServer::Start(const char * ip, int port)
{
	int result = 0;

	ZeroMemory(&baseInfo, sizeof(baseInfo));
	baseInfo.sin_family = AF_INET;
	baseInfo.sin_port = htons(port);
	inet_pton(AF_INET, ip, &(baseInfo.sin_addr.s_addr));

	result = bind(baseSocket, (sockaddr*)&baseInfo, sizeof(baseInfo));
	if (result == SOCKET_ERROR)
	{
		errorCode = WSAGetLastError();
		return 1;
	}

	result = listen(baseSocket, SOMAXCONN);
	if (result == SOCKET_ERROR)
	{
		errorCode = WSAGetLastError();
		closesocket(baseSocket);
		return 1;
	}
	serverState = true;
	return 0;
}

int NetworkServer::Accept()
{
	CLIENTINFO temp;
	int len = sizeof(temp.client);
	temp.s = accept(baseSocket, (sockaddr*)&temp.client, &len);
	if (temp.s == INVALID_SOCKET)
	{
		errorCode = WSAGetLastError();
		return 1;
	}
	inet_ntop(AF_INET, &temp.client.sin_addr, temp.Address, INET_ADDRSTRLEN);
	clientList.push_back(temp);
	return 0;
}

int NetworkServer::Recv(int index, char * recvData, int len)
{
	return recv(clientList.at(index).s,recvData,len,0);
}

void NetworkServer::RecvAll(char* recvData, int size)
{
	int result = 0;
	for (int i = 0; i < (int)clientList.size(); i++)
	{
		result = recv(clientList.at(i).s, recvData, size, 0);
		if (result == SOCKET_ERROR || result == 0)
		{
			RemoveClient(i);
		}
	}
}

void NetworkServer::SendAll(char* sendData,int size)
{
	int result = 0;
	for (int i = 0; i < (int)clientList.size(); i++)
	{
		result = send(clientList.at(i).s, sendData, size, 0);
		if (result == SOCKET_ERROR)
		{
			RemoveClient(i);
		}
	}
}

void NetworkServer::RemoveClient()
{
	for (int i = 0; i < (int)clientList.size(); i++)
	{
		RemoveClient(i);
	}
}

void NetworkServer::RemoveClient(int index)
{
	if (clientList.at(index).s != NULL)
	{
		closesocket(clientList.at(index).s);
		clientList.erase(clientList.begin()+index);
	}
}

char* NetworkServer::GetClientIP()
{
	return clientList.back().Address;
}

int NetworkServer::GetClientCount()
{
	return clientList.size();
}

bool NetworkServer::GetServerState()
{
	return serverState;
}

void NetworkServer::Close()
{
	//RemoveClient();
	shutdown(clientList[0].s,SD_SEND);
	serverState = false;
	WSACleanup();
	closesocket(baseSocket);
}
