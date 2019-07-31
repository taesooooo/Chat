#pragma warning(disable : 4996)

#include <iostream>
#include <thread>
#include <string>
#include "../Network/NetworkServer.h"
#include <windows.h>

#define PORT 27016
#define IP "127.0.0.1"
using namespace std;

NetworkServer *nr;
char* errorMsg;
void AcceptStart();
void RecvStart();

void Error()
{
	nr->Error(WSAGetLastError(), &errorMsg);
	cout << "오류 " << errorMsg;
	delete errorMsg;
}

void Output(string data) {

	char inputText[255] = { '\0' };
	int result = 0;
	int len = 0;
	int textLen = 0;
	COORD currentCursor, nextCursor;
	CONSOLE_SCREEN_BUFFER_INFO cursorInfo;
	GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursorInfo);

	/*if (cursorInfo.dwCursorPosition.X != 7)*/

	currentCursor.X = 7;
	currentCursor.Y = cursorInfo.dwCursorPosition.Y;

	textLen = cursorInfo.dwCursorPosition.X - 7;
	//inputText = new char[textLen + 1]{ '\0' };
	result = ReadConsoleOutputCharacterA(GetStdHandle(STD_OUTPUT_HANDLE), inputText, textLen, currentCursor, (LPDWORD)&len);
	result = FillConsoleOutputCharacterA(GetStdHandle(STD_OUTPUT_HANDLE), ' ', cursorInfo.dwSize.X, currentCursor, (LPDWORD)&len);

	nextCursor.X = 0;
	nextCursor.Y = cursorInfo.dwCursorPosition.Y;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), nextCursor);

	cout << data << endl;
	cout << "입력 >>" << inputText;
}

int main()
{
	int result = 0;
	char input[255];
	try
	{
		nr = new NetworkServer();
	}
	catch (std::runtime_error &e)
	{
		cout << "초기화 실패 " << e.what() << endl;
		getchar();
		return 0;
	}
	nr->Start(IP,PORT);

	thread th(AcceptStart);

	thread th2(RecvStart);

	while (1)
	{
		std::cout << "입력 >>";
		std::cin >> input;
		while (getchar() != '\n');
		if (strcmp(input,"/stop") == 0)
		{
			nr->Close();
			break;
		}
		else
		{
			nr->SendAll(input, 30);
		}
	}

	th.join();
	th2.join();

	delete nr;

	getchar();
	return 0;
}

void AcceptStart()
{
	string data;
	std::cout << "서버 시작" << std::endl;
	while (1)
	{
		if (nr->GetServerState() == false)
			break;

		int result = nr->Accept();
		if (result != 0)
		{
			Error();
			continue;
		}
			data = "클라이언트 연결됨 " + (string)nr->GetClientIP();
			Output(data);
	}
}

void RecvStart()
{
	char recvData[30];
	string data;
	int result = 0;
	while (1)
	{
		if (nr->GetServerState() == false)
			break;

		for (int i = 0; i < nr->GetClientCount(); i++)
		{
				result = nr->Recv(i, (char*)&recvData, sizeof(recvData));
				if (result == SOCKET_ERROR || result == 0)
				{
					nr->RemoveClient(i);
					continue;
				}
				data = "받음 >>" + (string)recvData + '\0';
				Output(data);
		}
	}
}