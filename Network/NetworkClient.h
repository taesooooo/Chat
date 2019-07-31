#pragma once
#include "NetworkBase.h"
class NetworkClient : public NetworkBase
{
private:
	CLIENTINFO clientInfo;
	bool connectState = false;
public:
	NetworkClient();
	~NetworkClient();
	int Start(const char* IP,int PORT);
	bool GetConnectState();
	void SetConnectState(bool state);
	char* GetClientIP();
};

