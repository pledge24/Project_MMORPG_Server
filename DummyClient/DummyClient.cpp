#include "pch.h"
#include <thread>

#define OUT

int main(void)
{
	// Winsock Init
	WSAData wsaData;
	if (::WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
		return 0;

	// 소켓 생성
	SOCKET clientSocket = ::socket(AF_INET, SOCK_STREAM, 0);
	if (clientSocket == INVALID_SOCKET)
		return 0;

	// 논블로킹 설정
	u_long on = 1;
	if(::ioctlsocket(clientSocket, FIONBIO, &on) == INVALID_SOCKET)
		return 0;

	// 서버 주소 설정
	SOCKADDR_IN serverAddr;
	::memset(&serverAddr, 0, sizeof(serverAddr));
	serverAddr.sin_family = AF_INET;
	::inet_pton(AF_INET, "127.0.0.1", OUT &serverAddr.sin_addr);
	serverAddr.sin_port = ::htons(7777);

	// Connect
	while (::connect(clientSocket, (SOCKADDR*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR)
	{
		// 논블로킹 처리
		if (::WSAGetLastError() == WSAEWOULDBLOCK)
			continue;

		// 이미 연결됨
		if (::WSAGetLastError() == WSAEISCONN)
			break;
	}

	cout << "Connected to Server" << endl;

	//char sendBuffer[1000] = "Hello World";
	//WSAEVENT wsaEvent = ::WSACreateEvent();
	//WSAOVERLAPPED overlapped = {};
	//overlapped.hEvent = wsaEvent;

	//// Send(Echo)
	//while (true)
	//{
	//	WSABUF wsaBuf;
	//	wsaBuf.buf = sendBuffer;
	//	wsaBuf.len = 1000;

	//	DWORD numberOfBytesSent = 0;
	//	DWORD flags = 0;

	//	// Register Send
	//	if (::WSASend(clientSocket, &wsaBuf, 1, &numberOfBytesSent, flags, &overlapped, nullptr) == SOCKET_ERROR)
	//	{
	//		// Pending
	//		if (::WSAGetLastError() == WSA_IO_PENDING)
	//		{
	//			::WSAWaitForMultipleEvents(1, &wsaEvent, TRUE, WSA_INFINITE, FALSE);
	//			::WSAGetOverlappedResult(clientSocket, &overlapped, &numberOfBytesSent, FALSE, &flags);
	//		}
	//		else
	//		{
	//			// TODO: 오류 처리
	//			// ...
	//			break;
	//		}
	//	}

	//	cout << "Send Data ! Len = " << numberOfBytesSent << endl;

	//	this_thread::sleep_for(1s);

	//}

	return 0;
}