#include "pch.h"
#include <thread>
#include <string>

#define OUT

void SendHelloWorld(SOCKET& clientSocket)
{
	// Send Hello World for 1s.
	while (true)
	{
		string sendBuffer = "Hello World";
		WSAEVENT wsaEvent = ::WSACreateEvent();
		WSAOVERLAPPED overlapped = {};
		overlapped.hEvent = wsaEvent;

		WSABUF wsaBuf;
		wsaBuf.buf = const_cast<char*>(sendBuffer.data());
		wsaBuf.len = sendBuffer.size();

		DWORD numOfBytes = 0;
		DWORD flags = 0;

		// Register Send
		if (::WSASend(clientSocket, &wsaBuf, 1, &numOfBytes, flags, &overlapped, nullptr) == SOCKET_ERROR)
		{
			// Pending
			//if (::WSAGetLastError() == WSA_IO_PENDING)
			//{
			//	::WSAWaitForMultipleEvents(1, &wsaEvent, TRUE, WSA_INFINITE, FALSE);
			//	::WSAGetOverlappedResult(clientSocket, &overlapped, &numberOfBytesSent, FALSE, &flags);
			//}
			//else
			//{
			//	// TODO: 오류 처리
			//	// ...
			//	break;
			//}
		}

		cout << "Send Data ! Len = " << numOfBytes << endl;

		this_thread::sleep_for(1s);

	}
}

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

	// send Thread;
	thread t1(SendHelloWorld, ref(clientSocket));


	// Recv(Main Thread)
	while (true)
	{	
		char recvBuffer[1000];

		WSABUF wsaBuf;
		wsaBuf.buf = recvBuffer;
		wsaBuf.len = 1000;

		DWORD numberOfBytesRecv = 0;
		DWORD flags = 0;

		WSAEVENT wsaEvent = ::WSACreateEvent();
		WSAOVERLAPPED overlapped = {};
		overlapped.hEvent = wsaEvent;

		// Register recv
		if (::WSARecv(clientSocket, &wsaBuf, 1, &numberOfBytesRecv, &flags, &overlapped, NULL) == SOCKET_ERROR)
		{
			// Pending
			int errCode = WSAGetLastError();
			if (errCode == WSA_IO_PENDING)
			{
				::WSAWaitForMultipleEvents(1, &wsaEvent, TRUE, WSA_INFINITE, FALSE);
				::WSAGetOverlappedResult(clientSocket, &overlapped, &numberOfBytesRecv, FALSE, &flags);
			}
			else
			{
				// TODO: 오류 처리
				// ...
				break;
			}
		}

		string recvStr(recvBuffer);
		cout << "complete to recv data = " << recvStr << endl;
	}

	return 0;
}