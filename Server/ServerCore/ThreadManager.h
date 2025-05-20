#pragma once

#include <thread>
#include <functional>
#include <mutex>
#include <vector>

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

