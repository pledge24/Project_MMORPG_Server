#pragma once
class Listener
{
public:
	Listener();
	~Listener();

	bool StartAccept();


private:
	SOCKET _listenSocket;
	ServerService* _service;
};

