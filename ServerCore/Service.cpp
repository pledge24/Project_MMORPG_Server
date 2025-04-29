#include "pch.h"
#include "Service.h"

ServerService::ServerService(NetAddress serverAddr, IocpCoreRef core, int32 maxSessionsCount):
	_serverAddr(serverAddr), _core(core), _maxSessionsCount(maxSessionsCount)
{
}

bool ServerService::Start()
{
	_listener->StartListen();
	_listener->StartAccept();

	return true;
}
