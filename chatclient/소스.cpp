#include <iostream>
#include <thread>
#include <string>
#include "../Network/NetworkClient.h"
#include <Windows.h>

#define PORT 27016
#define IP "127.0.0.1"
using namespace std;

NetworkClient *nc;
char *errorMsg;
void RecvStart();

void Error()
{
	nc->Error(WSAGetLastError(), &errorMsg);
	cout << "오류 " << errorMsg;
	delete errorMsg;
}

void Output(string data) {

	//char inputText[255] = { '\0' };
	//int result = 0;
	//int len = 0;
	//int textLen = 0;
	//COORD currentCursor, nextCursor;
	//CONSOLE_SCREEN_BUFFER_INFO cursorInfo;
	//GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursorInfo);

	///*if (cursorInfo.dwCursorPosition.X != 7)*/

	//currentCursor.X = 7;
	//currentCursor.Y = cursorInfo.dwCursorPosition.Y;

	//textLen = cursorInfo.dwCursorPosition.X - 7;
	////inputText = new char[textLen + 1]{ '\0' };
	//result = ReadConsoleOutputCharacterA(GetStdHandle(STD_OUTPUT_HANDLE), inputText, textLen, currentCursor, (LPDWORD)&len);
	//result = FillConsoleOutputCharacterA(GetStdHandle(STD_OUTPUT_HANDLE), ' ', cursorInfo.dwSize.X, currentCursor, (LPDWORD)&len);

	//nextCursor.X = 0;
	//nextCursor.Y = cursorInfo.dwCursorPosition.Y;
	//SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), nextCursor);

	cout << data << endl;
	cout << "입력 >>";
}

int main()
{
	int result = 0;
	char input[255];
	try
	{
		nc = new NetworkClient();
	}
	catch (std::runtime_error &e)
	{
		cout << "초기화 실패 " << e.what() << endl;
		getchar();
		return 0;
	}
	result = nc->Start(IP, PORT);
	if (result == SOCKET_ERROR)
	{
		Error();
		getchar();
		return 0;
	}
	thread th(RecvStart);
	
	while (1)
	{
		std::cout << "입력 >>";
		std::cin >> input;
		while (getchar() != '\n');
		if (strcmp(input, "/stop") == 0)
		{
			nc->SetConnectState(false);
			nc->Close();
			break;
		}
		else
		{
			result = nc->Send(input,30);
			if (result == SOCKET_ERROR)
			{
				Error();
			}
		}
	}

	th.join();

	delete nc;

	getchar();
	return 0;
}

void RecvStart()
{
	int result = 0;
	char recvData[30];
	string data;
	while (nc->GetConnectState())
	{
		result = nc->Recv(recvData, sizeof(recvData));
		switch (result)
		{
		case 0:
			cout << "서버가 종료되었습니다." << endl;
			nc->SetConnectState(false);
			break;
		case SOCKET_ERROR:
			if (nc->GetConnectState() == false)
				break;
			Error();
			break;
		default:
			data = recvData;
			Output(data);
			break;
		}
	}
}