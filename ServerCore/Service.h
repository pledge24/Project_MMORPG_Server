#pragma once
#include "Listener.h"
#include "NetAddress.h"

enum class ServiceType
{
	/*Client는 1~99*/


	/*Server는 100~*/
	SERVER_IOCP = 100
};

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
	ServerService(NetAddress serverAddr, ServiceType serverType);

private:
	Listener _listener;
	NetAddress _serverAddr;
};
