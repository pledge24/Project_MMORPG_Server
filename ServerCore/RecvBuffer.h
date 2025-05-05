#pragma once
#include <vector>

class RecvBuffer
{
public:
	RecvBuffer(int bufSize);
	~RecvBuffer();

	BYTE* Data() { return _buffer.data(); }

private:
	vector<BYTE> _buffer;
};

