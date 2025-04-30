#pragma once

#include "IocpCore.h"
#include "NetAddress.h"

class Session : public IocpObject
{
	enum
	{
		BUFFER_SIZE = 0x10000, // 64KB
	};

public:
	Session();
	~Session();

public:
	/* 인터페이스 구현 */
	virtual HANDLE GetHandle() override;
	virtual void Dispatch(class NetworkEvent* networkEvent, int32 numOfBytes = 0) override;

	/* 외부 사용 */
	SOCKET GetSocket() { return _socket; }
	void SetNetAddress(NetAddress sockAddress) { _addr = sockAddress; }

public:
	/*소켓이 있으면 세션이 있고, 세션이 있으면 accept를 제외한 나머지 이벤트를 등록할 수 있다.*/

	/* 네트워크 이벤트 등록 */
	void RegisterConnect();
	void RegisterDisconnect();
	void RegisterRecv();
	void RegisterSend();

	/* 네트워크 이벤트 실행 */
	void ProcessConnect();
	void ProcessDisconnect();
	void ProcessRecv(NetworkEvent* networkEvent, int32 numOfBytes);
	void ProcessSend(int32 numOfBytes);

public:
	NetAddress _addr;
	SOCKET _socket = INVALID_SOCKET;
	char _recvBuffer[BUFFER_SIZE];
	int32 recvBytes = 0;
};
