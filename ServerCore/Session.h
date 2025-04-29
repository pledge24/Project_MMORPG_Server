#pragma once

#include "NetAddress.h"

class Session
{
public:
	SOCKET GetSocket() { return socket; }
	void SetNetAddress(NetAddress sockAddress) { _addr = sockAddress; }

public:
	void ProcessConnect() {};

public:
	NetAddress _addr;
	SOCKET socket = INVALID_SOCKET;
	char _recvBuffer[1000];
	int32 recvBytes = 0;
};
