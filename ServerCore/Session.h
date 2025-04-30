#pragma once

#include "NetAddress.h"

class Session
{
	enum
	{
		BUFFER_SIZE = 0x10000, // 64KB
	};

public:
	Session();

public:
	SOCKET GetSocket() { return socket; }
	void SetNetAddress(NetAddress sockAddress) { _addr = sockAddress; }

public:
	void ProcessConnect() {};

public:
	NetAddress _addr;
	SOCKET socket = INVALID_SOCKET;
	char _recvBuffer[BUFFER_SIZE] = {};
	int32 recvBytes = 0;
};
