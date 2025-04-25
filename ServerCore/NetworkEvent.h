#pragma once

enum class EventType 
{ 
	Connect, 
	Disconnect, 
	Accept, 
	Recv, 
	Send
};

/*-----------------
	NetworkEvent
-------------------*/

class NetworkEvent : public WSAOVERLAPPED
{
public:
	NetworkEvent(EventType eventType) : _eventType(eventType) {}

private:
	EventType _eventType;
};

/*-----------------
	ConnectEvent
-------------------*/

class ConnectEvent : public NetworkEvent
{
public:
	ConnectEvent() : NetworkEvent(EventType::Connect) {}
};

/*-------------------
	DisconnectEvent
--------------------*/

class DisconnectEvent : public NetworkEvent
{
public:
	DisconnectEvent() : NetworkEvent(EventType::Disconnect) {}
};

/*-------------------
	AcceptEvent
--------------------*/

class AcceptEvent : public NetworkEvent
{
public:
	AcceptEvent() : NetworkEvent(EventType::Accept) {}
};

/*--------------
	RecvEvent
----------------*/

class RecvEvent : public NetworkEvent
{
public:
	RecvEvent() : NetworkEvent(EventType::Recv) {}
};

/*--------------
	SendEvent
----------------*/

class SendEvent : public NetworkEvent
{
public:
	SendEvent() : NetworkEvent(EventType::Send) {}
};

