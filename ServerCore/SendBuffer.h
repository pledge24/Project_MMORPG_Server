#pragma once

/*-------------------
	  SendBuffer
--------------------*/

// TCP 송신 버퍼
class SendBuffer
{
public:
	SendBuffer(int32 bufferSize);
	~SendBuffer();

public:
	BYTE*			Data()								{ return _buffer.data(); }
	int32			Size()								{ return _buffer.size(); }
	int32			WriteSize()							{ return _writePos; }
	int32			FreeSize()							{ return _buffer.size() - _writePos; }
	BYTE*			WritePos()							{ return &_buffer[_writePos]; }

	void			Resize(int32 bufferSize);
	void			Clear();
	void			CopyData(void* data, int32 len);
	void			AppendData(void* data, int32 len);

private:
	vector<BYTE>	_buffer;
	int32			_writePos = 0; // data가 들어있는 다음 위치를 가리킴.
};

