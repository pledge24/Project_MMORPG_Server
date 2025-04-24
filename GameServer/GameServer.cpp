#include "pch.h"
#include <thread>
#include "Service.h"
#include "IocpCore.h"

#define BUFSIZE 1000

class Session
{
public:
	SOCKET socket = INVALID_SOCKET;
	char recvBuffer[1000];
	int32 recvBytes = 0;
};

enum class IO_TYPE { READ, WRITE, ACCEPT, CONNECT, DISCONNECT};

struct OverlappedEx
{
	WSAOVERLAPPED overlapped = {};
	IO_TYPE type;
};

void WorkerThreadMain(HANDLE iocpHandle)
{
	while (true)
	{
		DWORD bytesTransferred = 0;
		Session* session = nullptr;
		OverlappedEx* overlappedEx = nullptr;

		BOOL ret = ::GetQueuedCompletionStatus(iocpHandle, &bytesTransferred,
			(ULONG_PTR*)&session, (LPOVERLAPPED*)&overlappedEx, INFINITE);

		// 문제가 생긴 경우
		if (ret == FALSE || bytesTransferred == 0)
		{
			continue;
		}

		if (overlappedEx->type != IO_TYPE::READ)
		{
			cout << "This is not Read Task" << endl;
			continue;
		}

		cout << "Recv Data IOCP = " << bytesTransferred << endl;
		
		WSABUF wsaBuf;
		wsaBuf.buf = session->recvBuffer;
		wsaBuf.len = BUFSIZE;

		DWORD recvLen = 0;
		DWORD flags = 0;
		::WSARecv(session->socket, &wsaBuf, 1, &recvLen, &flags, &overlappedEx->overlapped, NULL);

	}
}

int main(void)
{
	ServerServiceRef service = make_shared<ServerService>(
		NetAddress("127.0.0.1"s, 7777),
		make_shared<IocpCore>()
	);

	ASSERT_CRASH(service->Start());

	// worker Thread
	const int threadN = 5;
	for (int i = 0; i < threadN; i++)
	{
		GThreadManager->Launch([=]()
			{
				WorkerThreadMain(iocpHandle);
			});
	}

	// Accept
	while (true)
	{
		SOCKADDR_IN clientAddr;
		int32 addrLen = sizeof(clientAddr);

		SOCKET clientSocket = ::accept(listenSocket, (SOCKADDR*)&clientAddr, &addrLen);
		if (clientSocket == INVALID_SOCKET)
			return 0;

		Session* session = new Session();
		session->socket = clientSocket;

		cout << "New Client Socket is Connected!" << endl;

		// Register New Client Socket in IOCP
		::CreateIoCompletionPort((HANDLE)clientSocket, iocpHandle, /*Key*/(ULONG_PTR)session, 0);

		// 처음 WSARecv등록
		WSABUF wsaBuf;
		wsaBuf.buf = session->recvBuffer;
		wsaBuf.len = BUFSIZE;

		OverlappedEx* overlappedEx = new OverlappedEx();
		overlappedEx->type = IO_TYPE::READ;

		DWORD recvLen = 0;
		DWORD flags = 0;
		::WSARecv(clientSocket, &wsaBuf, 1, &recvLen, &flags, &overlappedEx->overlapped, NULL);
	}

	GThreadManager->Join();

	return 0;
}