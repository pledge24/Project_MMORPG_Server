#include "pch.h"
#include "SendBuffer.h"

SendBuffer::SendBuffer(int32 bufferSize)
{
	_buffer.resize(bufferSize);
}

SendBuffer::~SendBuffer()
{
}

void SendBuffer::Resize(int32 bufferSize)
{
	_buffer.resize(bufferSize);
	_writePos = min(_writePos, bufferSize);
}

void SendBuffer::Clear()
{
	_buffer.clear();
	_writePos = 0;
}

void SendBuffer::CopyData(void* data, int32 len)
{
	Clear();
	Resize(len);
	::memcpy(_buffer.data(), data, len);
	_writePos = len;
}

void SendBuffer::AppendData(void* data, int32 len)
{
	if (FreeSize() < len)
		_buffer.resize(WriteSize() + len);
	::memcpy(WritePos(), data, len);
	_writePos += len;
}
