#pragma once

#include <queue>

template <typename T>
class CSafeQueue
{
public:
	CSafeQueue()
	{
		InitializeCriticalSection(&mCS);
		InitializeConditionVariable(&mCV);
	}

	//CSafeQueue(const CSafeQueue& other);
	//CSafeQueue& operator=(const CSafeQueue& other);

	~CSafeQueue()
	{
		DeleteCriticalSection(&mCS);
	}

	void Enqueue(T item)
	{
		EnterCriticalSection(&mCS);
		mQueue.push(item);
		LeaveCriticalSection(&mCS);
		WakeAllConditionVariable(&mCV);
	}

	T Dequeue()
	{
		EnterCriticalSection(&mCS);
		while (mQueue.empty())
			SleepConditionVariableCS(&mCV, &mCS, INFINITE);
		T ret = mQueue.front();
		mQueue.pop();
		LeaveCriticalSection(&mCS);
		return ret;
	}

private:
	std::queue<T> mQueue;
	CRITICAL_SECTION mCS;
	CONDITION_VARIABLE mCV;
};

