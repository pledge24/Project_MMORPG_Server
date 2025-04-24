#include "pch.h"
#include "Listener.h"
#include "NetAddress.h"
#include "Service.h"
#include "SocketUtil.h"

Listener::Listener()
{
}

Listener::~Listener()
{
}

bool Listener::StartAccept()
{
	_listenSocket = SocketUtil::CreateSocket();

	// Bind 
	if (SocketUtil::Bind(_listenSocket, _service->GetServerAddr()) == false)
		return false;
			
	// Listen
	if (SocketUtil::Listen(_listenSocket) == false)
		return false;

	cout << "Success to generate listen Socket" << endl;


	// ===[Accept]===

	/*const int32 acceptCount = _service->GetMaxSessionCount();
	for (int32 i = 0; i < acceptCount; i++)
	{
		AcceptEvent* acceptEvent = new AcceptEvent();
		acceptEvent->owner = shared_from_this();
		_acceptEvents.push_back(acceptEvent);
		RegisterAccept(acceptEvent);
	}*/

	return true;
}
