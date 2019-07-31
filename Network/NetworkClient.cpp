#include "NetworkClient.h"



NetworkClient::NetworkClient()
{
}


NetworkClient::~NetworkClient()
{

}

int NetworkClient::Start(const char* IP, int PORT)
{
	int result = 0;

	ZeroMemory(&baseInfo, sizeof(baseInfo));
	baseInfo.sin_family = AF_INET;
	baseInfo.sin_port = htons(PORT);
	inet_pton(AF_INET, IP, &(baseInfo.sin_addr.s_addr));

	result = connect(baseSocket, (sockaddr*)&baseInfo, sizeof(baseInfo));
	if (result == SOCKET_ERROR)
	{
		errorCode = WSAGetLastError();
		closesocket(baseSocket);
		WSACleanup();
		return result;
	}
	connectState = true;
	return result;
}

bool NetworkClient::GetConnectState()
{
	return connectState;
}

void NetworkClient::SetConnectState(bool state)
{
	connectState = state;
}
