#pragma once

/*----------------
	IocpObject
-----------------*/

/* Listener / Session 인터페이스 */
class IocpObject : public enable_shared_from_this<IocpObject>
{
public:
	virtual HANDLE GetHandle() abstract;
	virtual void Dispatch(class NetworkEvent* iocpEvent, int32 numOfBytes = 0) abstract;
};

/*---------------
	IocpCore
----------------*/
class IocpCore
{
public:
	IocpCore();
	~IocpCore();

public:
	bool		RegisterSocket();
	bool		Dispatch(uint32 timeoutMs = INFINITE);

private:
	HANDLE _iocpHandle;
};

