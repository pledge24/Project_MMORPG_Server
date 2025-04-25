#include "pch.h"
#include <thread>
#include "Service.h"
#include "IocpCore.h"

#define BUFSIZE 1000



void WorkerThreadMain(ServerServiceRef service)
{
	while (true)
	{
		DWORD bytesTransferred = 0;
		Session* session = nullptr;
		OverlappedEx* overlappedEx = nullptr;

		// CP에 들어있던 일감을 받을때까지 깬다.
		// 1) 일감을 받을 iocp 핸들, 2) 크기, 3) 넘겨줬던 키, 
		// 4) 내부에서 사용하는 OVERLAPPED 구조체 주소, 5) timeout
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
		GThreadManager->Launch([&service]()
			{
				WorkerThreadMain(service);
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

		// Regist New Client Socket in IOCP
		// 1) 클라이언트 소켓 핸들, 존재하는 IOCP 핸들, 키(세션 포인터), 0 
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