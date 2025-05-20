#include "pch.h"
#include "CoreGlobal.h"
#include "SocketUtil.h"

ThreadManager* GThreadManager = nullptr;

class CoreGlobal
{
public:
	CoreGlobal()
	{
		GThreadManager = new ThreadManager();
		SocketUtil::Init();
	}

	~CoreGlobal()
	{
		delete GThreadManager;
		SocketUtil::Clear();
	}

private:

} GCoreGlobal;