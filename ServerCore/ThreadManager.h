#pragma once

#include <thread>
#include <functional>
#include <mutex>    // mutex 사용을 위해 추가
#include <vector>   // vector 사용을 위해 추가

/*------------------
	ThreadManager
-------------------*/

using namespace std;

class ThreadManager
{
public:
	ThreadManager();
	~ThreadManager();

	void	Launch(function<void(void)> callback);
	void	Join();

private:
	mutex			_lock;
	vector<thread>	_threads;
};

