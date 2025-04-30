#include "pch.h"
#include "Service.h"
#include "SocketUtil.h"

ServerService::ServerService(NetAddress serverAddr, IocpCoreRef core, int32 maxSessionsCount):
	_serverAddr(serverAddr), _core(core), _maxSessionsCount(maxSessionsCount)
{
}

bool ServerService::Start()
{
	//ServerServiceRef service = static_pointer_cast<ServerService>(shared_from_this());
	_listener = make_shared<Listener>(shared_from_this());
	if (_listener == nullptr)
		return false;

	if (false == _listener->StartListen())
		return false;

	if (false == _listener->StartAccept())
		return false;

	return true;
}
