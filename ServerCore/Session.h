#pragma once
class Session
{
public:
	SOCKET GetSocket() { return socket; }

	SOCKET socket = INVALID_SOCKET;
	char _recvBuffer[1000];
	int32 recvBytes = 0;
};
