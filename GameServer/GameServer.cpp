#include "pch.h"
#include <thread>

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
	WSAData wsaData;
	if (::WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
		return 0;

	SOCKET listenSocket = ::socket(AF_INET, SOCK_STREAM, 0);
	if (listenSocket == INVALID_SOCKET)
		return 0;

	SOCKADDR_IN serverAddr;
	::memset(&serverAddr, 0, sizeof(serverAddr));
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_addr.s_addr = ::htonl(INADDR_ANY);
	serverAddr.sin_port = ::htons(7777);

	if (::bind(listenSocket, (SOCKADDR*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR)
		return 0;

	if (::listen(listenSocket, SOMAXCONN) == SOCKET_ERROR)
		return 0;

	cout << "Succes to generate listen Socket" << endl;


	// CP 생성
	HANDLE iocpHandle = ::CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, 0);


	// worker Thread
	vector<thread> workerThreads;
	const int threadN = 5;
	for (int i = 0; i < threadN; i++)
	{
		workerThreads.push_back(thread([=]()
			{
				WorkerThreadMain(iocpHandle);
			})
		);
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

	for (int i = 0; i < threadN; i++)
	{
		if (workerThreads[i].joinable())
		{
			workerThreads[i].join();
		}
	}

	return 0;
}