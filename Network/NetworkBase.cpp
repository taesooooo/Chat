#include "NetworkBase.h"

NetworkBase::NetworkBase()
{
	int result = 0;

	result = WSAStartup(MAKEWORD(2,2), &wsaData);
	if (result != 0)
	{
		throw std::runtime_error("초기화 오류");
	}

	baseSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (baseSocket == INVALID_SOCKET)
	{
		errorCode = WSAGetLastError();
		WSACleanup();
		throw std::runtime_error("소켓 초기화 오류");
	}
}

NetworkBase::~NetworkBase()
{

}

int NetworkBase::Recv(char* Data,int Size)
{
	return recv(baseSocket, Data, Size, 0);
}

int NetworkBase::Send(char* Data, int Size)
{
	return send(baseSocket, Data, Size, 0);
}

void NetworkBase::Error(int code ,char** msgbuf)
{
	LPWSTR buffer = NULL;
	FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM, NULL, code, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPWSTR)&buffer, 0, NULL);
	int len = WideCharToMultiByte(CP_ACP, 0, buffer, -1, NULL, 0, NULL, NULL); // buffer 문자 개수 반환으로 이용
	*msgbuf = new char[len];
	WideCharToMultiByte(CP_ACP, 0, buffer, -1, *msgbuf, len, NULL, NULL); // 유니코드인 buffer내용 변환
	LocalFree(buffer);// 생성했던 에러메세지 공간 해제
}

void NetworkBase::Close()
{
	shutdown(baseSocket, SD_SEND);
	closesocket(baseSocket);
	WSACleanup();
}
