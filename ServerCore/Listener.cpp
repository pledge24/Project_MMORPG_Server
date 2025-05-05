#include "pch.h"
#include "Listener.h"
#include "NetAddress.h"
#include "Service.h"
#include "SocketUtil.h"
#include "NetworkEvent.h"
#include "Session.h"

Listener::Listener(ServerServiceRef service): _service(service)
{
}

Listener::~Listener()
{
}

HANDLE Listener::GetHandle()
{
	return reinterpret_cast<HANDLE>(_listenSocket);
}

void Listener::Dispatch(NetworkEvent* networkEvent, int32 numOfBytes)
{
	cout << "UM" << '\n';
	ASSERT_CRASH(networkEvent->eventType == EventType::Accept);
	AcceptEvent* acceptEvent = static_cast<AcceptEvent*>(networkEvent);
	ProcessAccept(acceptEvent);
}

bool Listener::StartListen()
{
	// Socket
	_listenSocket = SocketUtil::CreateSocket();
	if (_listenSocket == INVALID_SOCKET)
		return false;

	// Bind 
	if (SocketUtil::Bind(_listenSocket, _service->GetServerAddr()) == false)
		return false;

	// Listen
	if (SocketUtil::Listen(_listenSocket) == false)
		return false;

	cout << "Success to generate listen Socket" << endl;

	return true;
}

bool Listener::StartAccept()
{
	if (_service->GetIocpCore()->RegisterSocket(_listenSocket) == false)
		return false;

	const int32 acceptCount = _service->GetMaxSessionCount();
	for (int32 i = 0; i < acceptCount; i++)
	{
		AcceptEvent* acceptEvent = new AcceptEvent();
		acceptEvent->owner = shared_from_this();
		_acceptEvents.push_back(acceptEvent);
		RegisterAccept(acceptEvent);
	}

	cout << "Success to register AcceptEvent: " << acceptCount << endl;

	return true;
}

void Listener::RegisterAccept(AcceptEvent* acceptEvent)
{
	SessionRef session = make_shared<Session>();
	_service->GetIocpCore()->RegisterSocket(session->GetSocket());

	acceptEvent->session = session;

	DWORD bytesReceived = 0;
	if (false == SocketUtil::AcceptEx(_listenSocket, session->GetSocket(), session->_recvBuffer.Data(), 0, sizeof(SOCKADDR_IN) + 16,
		sizeof(SOCKADDR_IN) + 16, OUT &bytesReceived, static_cast<LPOVERLAPPED>(acceptEvent)))
	{
		const int32 errorCode = ::WSAGetLastError();
		if (errorCode != WSA_IO_PENDING)
		{
			// 일단 다시 Accept 걸어준다
			RegisterAccept(acceptEvent);
		}
	}
}

void Listener::ProcessAccept(AcceptEvent* acceptEvent)
{
	cout << "New Client Arrived" << endl;

	SessionRef session = acceptEvent->session;

	if (false == SocketUtil::SetUpdateAcceptSocket(session->GetSocket(), _listenSocket))
	{
		RegisterAccept(acceptEvent);
		return;
	}

	SOCKADDR_IN sockAddress;
	int32 sizeOfSockAddr = sizeof(sockAddress);
	if (SOCKET_ERROR == ::getpeername(session->GetSocket(), OUT reinterpret_cast<SOCKADDR*>(&sockAddress), &sizeOfSockAddr))
	{
		RegisterAccept(acceptEvent);
		return;
	}

	session->SetNetAddress(NetAddress(sockAddress));
	session->ProcessConnect();

	RegisterAccept(acceptEvent);
}
