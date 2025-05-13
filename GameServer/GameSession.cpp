#include "pch.h"
#include "GameSession.h"

void GameSession::OnConnected()
{
}

void GameSession::OnDisconnected()
{
}

int32 GameSession::OnRecv(BYTE* buffer, int32 len)
{
	cout << "complete to receiced data: " << buffer << endl;

	SendBufferRef sendBuffer = make_shared<SendBuffer>(len);
	sendBuffer->CopyData(buffer, len);
	Send(sendBuffer);
	return len;
}

void GameSession::OnSend(int32 len)
{
	cout << "complete to send data = " << len << endl;
}
