#pragma once
#include <vector>

/*-------------------
	  RecvBuffer
--------------------*/

// TCP 수신 버퍼
class RecvBuffer
{
	enum { DEFAULT_MULTIPLE_N = 10, MIN_MULTIPLE_N = 2};
	enum { DEFAULT_CHUNKSIZE = 0x10000};

public:
	RecvBuffer(int32 chunkSize = DEFAULT_CHUNKSIZE/*64KB*/, int32 multipleN = DEFAULT_MULTIPLE_N);
	~RecvBuffer();

					/* RecvBuffer 커서 관련*/
	void			Clean();
	bool			OnRead(int32 numOfBytes);
	bool			OnWrite(int32 numOfBytes);

					/* RecvBuffer 정보 관련*/
	BYTE*			Data()			{ return _buffer.data(); }
	BYTE*			ReadPos()		{ return &_buffer[_readPos]; }
	BYTE*			WritePos()		{ return &_buffer[_writePos]; }
	int32			Size()			{ return (int32)_buffer.size(); }
	int32			UnreadSize()	{ return _writePos - _readPos; }
	int32			FreeSize()		{ return _bufferSize - _writePos; }

private:
	vector<BYTE>	_buffer;
	int32			_chunkSize = 0;
	int32			_multipleN = 0;
	int32			_bufferSize = 0; 
	int32			_readPos = 0;
	int32			_writePos = 0;
};

