#include "pch.h"
#include "Session.h"
#include "SocketUtil.h"
#include "NetworkEvent.h"

Session::Session() : _recvBuffer(BUFFER_SIZE)
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
	wsaBuf.buf = reinterpret_cast<char*>(_recvBuffer.Data());
	wsaBuf.len = BUFFER_SIZE;

	DWORD numOfBytes = 0;
	DWORD flags = 0;

	RecvEvent* recvEvent = new RecvEvent();
	recvEvent->owner = static_pointer_cast<Session>(shared_from_this());

	::WSARecv(_socket, &wsaBuf, 1, &numOfBytes, &flags, static_cast<LPWSAOVERLAPPED>(recvEvent), NULL);
}

// 직렬화(serialized)된 패킷을 받아서 비동기 송신.
void Session::RegisterSend(vector<BYTE> packet)
{
	WSABUF sendBuf;
	sendBuf.buf = reinterpret_cast<char*>(packet.data());
	sendBuf.len = packet.size();

	DWORD numOfBytes = 0;
	DWORD flags = 0;

	SendEvent* sendEvent = new SendEvent();
	sendEvent->owner = static_pointer_cast<Session>(shared_from_this());

	::WSASend(_socket, &sendBuf, 1, &numOfBytes, flags, static_cast<LPWSAOVERLAPPED>(sendEvent), NULL);
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
	vector<BYTE> recvData(numOfBytes);
	memcpy(recvData.data(), _recvBuffer.Data(), numOfBytes);

	
	cout << "Client recieve data = " << (char*)recvData.data() << endl;
	delete networkEvent;

	// ping pong
	RegisterSend(recvData);

	// 3초 정도 sleep(TCP가 뭉쳐서 recv받는거 체크)
	this_thread::sleep_for(3000ms);

	// 현재 세션의 recv 이벤트 재등록.
	RegisterRecv();

}

void Session::ProcessSend(int32 numOfBytes)
{
	cout << "complete to send data = " << numOfBytes << endl;
}
