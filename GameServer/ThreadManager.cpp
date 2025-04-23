#include "pch.h"
#include "ThreadManager.h"

ThreadManager::ThreadManager()
{
}

ThreadManager::~ThreadManager()
{
    Join();
}

void ThreadManager::Launch(function<void(void)> callback)
{
	_threads.push_back(thread([=]()
		{
            // Launch시 실행할 함수 추가
			callback();
		}));
}

void ThreadManager::Join()
{
    for (thread& t : _threads)
    {
        if (t.joinable())
            t.join();
    }
    _threads.clear();
}
