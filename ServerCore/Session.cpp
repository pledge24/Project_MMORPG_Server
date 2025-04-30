#include "pch.h"
#include "Session.h"
#include "SocketUtil.h"

Session::Session()
{
	socket = SocketUtil::CreateSocket();
}
