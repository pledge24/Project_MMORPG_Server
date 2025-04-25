#pragma once

class AcceptEvent;

class Listener : public IocpObject
{
public:
	Listener();
	~Listener();

public:
	/* 인터페이스 구현 */
	virtual HANDLE GetHandle() override;
	virtual void Dispatch(class NetworkEvent* iocpEvent, int32 numOfBytes = 0) override;

	bool StartAccept();

	/* 수신 관련 */
	void RegisterAccept(AcceptEvent* acceptEvent);
	void ProcessAccept(AcceptEvent* acceptEvent);

private:
	SOCKET _listenSocket;
	ServerService* _service;
	vector<AcceptEvent*> _acceptEvents;
};

