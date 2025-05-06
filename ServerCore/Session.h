#pragma once

#include "IocpCore.h"
#include "NetAddress.h"
#include "RecvBuffer.h"
#include "SendBuffer.h"
#include <vector>

class Session : public IocpObject
{
	friend class Listener;

	enum
	{
		BUFFER_SIZE = 0x10000, // 64KB
	};

public:
	Session();
	~Session();

public:
							/* 인터페이스 구현 */
	virtual HANDLE			GetHandle() override;
	virtual void			Dispatch(class NetworkEvent* networkEvent, int32 numOfBytes = 0) override;

public:
							/* 통신 관련 */
	void					Send(SendBufferRef sendBuffer);
	bool					Connect();								// Only-use Client
	void					Disconnect(const WCHAR* cause);

public:
							/* 정보 관련 */
	void					SetNetAddress(NetAddress address) { _netAddress = address; }
	NetAddress				GetAddress() { return _netAddress; }
	SOCKET					GetSocket() { return _socket; }
	bool					IsConnected() { return _connected; }
	SessionRef				GetSessionRef() { return static_pointer_cast<Session>(shared_from_this()); }
	
private:
							/* 네트워크 이벤트 등록 */
	bool					RegisterConnect();						// Only-use Client
	void					RegisterDisconnect();
	void					RegisterRecv();
	void					RegisterSend(vector<BYTE> packet);

							/* CP 완료 패킷 처리 */
	void					ProcessConnect();
	void					ProcessDisconnect();
	void					ProcessRecv(NetworkEvent* networkEvent, int32 numOfBytes);
	void					ProcessSend(int32 numOfBytes);

private:
	NetAddress				_netAddress;
	SOCKET					_socket = INVALID_SOCKET;
	BOOL					_connected = false;

private:
	RecvBuffer				_recvBuffer;

							/* sendEvent 관련 */
	vector<SendBufferRef>	_sendQueue;
	//SendBuffer				_sendBuffer;
	atomic<BOOL>			_sendRegistered;
};
