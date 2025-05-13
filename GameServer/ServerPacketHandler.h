#pragma once
#include "Protocol.pb.h"
#include "SendBuffer.h"
#include "Session.h"

class ServerPacketHandler
{
public:
	static void HandlePacket(BYTE* buffer, int32 len);

	static SendBufferRef MakeSendBuffer(Protocol::S_TEST& pkt);
	// 새로운 패킷이 생길때마다 추가..
};

template<typename T>
SendBufferRef _MakeSendBuffer(T& pkt, uint16 pktId)
{
	const uint16 dataSize = static_cast<uint16>(pkt.ByteSizeLong());
	const uint16 packetSize = dataSize + sizeof(PacketHeader);

	SendBufferRef sendBuffer = make_shared<SendBuffer>(packetSize);
	PacketHeader* header = reinterpret_cast<PacketHeader*>(sendBuffer->Data());
	header->id = pktId;
	header->size = dataSize;
	ASSERT_CRASH(pkt.SerializeToArray(header + sizeof(PacketHeader), dataSize));

	return sendBuffer;
}


