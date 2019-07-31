#include "server.h"

server::server(const char *ip, int port)
{
	ZeroMemory(&serverInfo, sizeof(serverInfo));
	serverInfo.sin_family = AF_INET;
	serverInfo.sin_port = htons(port);
	inet_pton(AF_INET, ip, &(serverInfo.sin_addr.s_addr));
}

server::~server()
{

}

void server::ServerStart()
{
	int result = 0;

	std::cout << "서버 스타트업!" << std::endl;
	result = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (result != 0)
	{
		Error(WSAGetLastError());
	}

	std::cout << "서버 소켓 생성" << std::endl;
	listenSocket = socket(serverInfo.sin_family, SOCK_STREAM, IPPROTO_TCP);
	if (listenSocket == INVALID_SOCKET)
	{
		Error(WSAGetLastError());
		WSACleanup();
	}

	std::cout << "서버 등록" << std::endl;
	 result = bind(listenSocket, (sockaddr*)&serverInfo, sizeof(serverInfo));
	if (result == SOCKET_ERROR)
	{
		Error(WSAGetLastError());
		WSACleanup();
	}
	
	std::cout << "클라이언트 요청 대기" << std::endl;
	result = listen(listenSocket, SOMAXCONN);
	if (result == SOCKET_ERROR)
	{
		Error(WSAGetLastError());
		closesocket(listenSocket);
		WSACleanup();
	}
	std::cout << "-서버 시작-" << std::endl;
	std::cout << "클라이언트 연결 기다리는중..." << std::endl;
}

int server::Accpte()
{
	clientSocket = accept(listenSocket, (sockaddr*)&clientInfo, &sockLen);
	if (clientSocket == INVALID_SOCKET)
	{
		Error(WSAGetLastError());
		closesocket(listenSocket);
		return 1;
	}
	socketList.push_back(clientSocket);
	return 0;
}

int server::SendAll(char * sendData, int size)
{
	for (int i = 0; i < (int)socketList.size(); i++)
	{
		send(socketList.at(i), sendData, size, 0);
	}
	return 0;
}

int server::DataRecv(SOCKET s,char* recvData,int size)
{
	return recv(s, recvData, size, 0);
}

int server::DataSend(SOCKET s, char *recvData, int size)
{
	return send(s, recvData, size, 0);
}

void server::GetClientIP(char* getip)
{
	inet_ntop(AF_INET, &clientInfo.sin_addr, getip, INET_ADDRSTRLEN);
	//getpeername(clientSocket, (sockaddr*)&clientInfo.sin_addr, &sockLen);
}

SOCKET server::GetClientSocket()
{
	return socketList.back();
}

void server::RemoveClient(SOCKET s)
{
	if (!socketList.empty())
	{
		vector<SOCKET>::iterator pos;
		for (pos = socketList.begin(); pos != socketList.end(); pos++)
		{
			if (*pos == s)
			{
				socketList.erase(pos);
			}
		}
	}
}

void server::Error(int error)
{
	char *msg;
	LPWSTR buffer = NULL;
	FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM, NULL, error, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPWSTR)&buffer, 0, NULL);
	int len = WideCharToMultiByte(CP_ACP, 0, buffer, -1, NULL, 0, NULL, NULL); // buffer 문자 개수 반환으로 이용
	msg = new char[len];
	WideCharToMultiByte(CP_ACP, 0, buffer, -1, msg, len, NULL, NULL); // 유니코드인 buffer내용 변환
	std::cout << "에러 코드:" << error << " " << msg << std::endl;
	LocalFree(buffer);// 생성했던 에러메세지 공간 해제
}

void server::Close()
{
}
