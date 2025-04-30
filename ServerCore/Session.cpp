#include "pch.h"
#include "Session.h"
#include "SocketUtil.h"
#include "NetworkEvent.h"

Session::Session()
{
	_socket = SocketUtil::CreateSocket();
}

Session::~Session()
{
}

HANDLE Session::GetHandle()
{
	return reinterpret_cast<HANDLE>(_socket);
}

void Session::Dispatch(NetworkEvent* networkEvent, int32 numOfBytes)
{
	switch (networkEvent->eventType)
	{
	case EventType::Connect:
		// TODO
		break;
	case EventType::Disconnect:
		// TODO
		break;
	case EventType::Recv:
		ProcessRecv(networkEvent, numOfBytes);
		break;
	case EventType::Send:
		ProcessSend(numOfBytes);
		break;
	default:
		// TODO
		break;
	}
}

void Session::RegisterConnect()
{
}

void Session::RegisterDisconnect()
{
}

void Session::RegisterRecv()
{
	WSABUF wsaBuf;
	wsaBuf.buf = _recvBuffer;
	wsaBuf.len = BUFFER_SIZE;

	RecvEvent* recvEvent = new RecvEvent();
	recvEvent->owner = static_pointer_cast<Session>(shared_from_this());

	DWORD numOfBytes = 0;
	DWORD flags = 0;
	::WSARecv(_socket, &wsaBuf, 1, &numOfBytes, &flags, static_cast<LPWSAOVERLAPPED>(recvEvent), NULL);
}

void Session::RegisterSend()
{
	
}

void Session::ProcessConnect()
{
	RegisterRecv();
}

void Session::ProcessDisconnect()
{
}

void Session::ProcessRecv(NetworkEvent* networkEvent, int32 numOfBytes)
{
	cout << "Client recieve data = " << _recvBuffer << endl;
	delete networkEvent;

	// 현재 세션의 recv 이벤트 재등록.
	RegisterRecv();
}

void Session::ProcessSend(int32 numOfBytes)
{
}
