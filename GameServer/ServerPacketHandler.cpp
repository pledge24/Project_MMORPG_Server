#include "pch.h"
#include "ServerPacketHandler.h"

void ServerPacketHandler::HandlePacket(BYTE* buffer, int32 len)
{
}

SendBufferRef ServerPacketHandler::MakeSendBuffer(Protocol::S_TEST& pkt)
{
	return SendBufferRef();
}
