#pragma once
#include "IocpCore.h"

class AcceptEvent;
class ServerService;

class Listener : public IocpObject
{
public:
	Listener();
	~Listener();

public:
	/* 인터페이스 구현 */
	virtual HANDLE GetHandle() override;
	virtual void Dispatch(class NetworkEvent* networkEvent, int32 numOfBytes = 0) override;

	bool StartListen();
	bool StartAccept();

	/* 수신 관련 */
	void RegisterAccept(AcceptEvent* acceptEvent);
	void ProcessAccept(AcceptEvent* acceptEvent);

private:
	SOCKET _listenSocket;
	ServerServiceRef _service;
	vector<AcceptEvent*> _acceptEvents;
};

