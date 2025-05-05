#include "pch.h"
#include "RecvBuffer.h"

RecvBuffer::RecvBuffer(int bufSize)
{
	_buffer.reserve(bufSize);
}

RecvBuffer::~RecvBuffer()
{
}
