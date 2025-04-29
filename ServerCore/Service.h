#pragma once
#include "Listener.h"
#include "NetAddress.h"

//enum class ServiceType
//{
//	Client,
//	Server
//};

//class Service
//{
//};
//
//class ClientService : public Service
//{
//
//};

/*-----------------
	ServerService
------------------*/

// 일단 IOCP 모델 고정으로 생각.
class ServerService
{
public:
	ServerService(NetAddress serverAddr, IocpCoreRef core, int32 maxSessionsCount = SOMAXCONN);

	NetAddress GetServerAddr() { return _serverAddr; }

	bool Start();

	int32 GetMaxSessionCount() { return _maxSessionsCount; }
	IocpCoreRef GetIocpCore() { return _core; }

private:
	NetAddress _serverAddr;
	IocpCoreRef _core;
	int32 _maxSessionsCount;
	ListenerRef _listener;
};
