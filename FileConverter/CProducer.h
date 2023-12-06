#pragma once
#include "CSafeQueue.h"

class CProducer
{
public:
	CProducer(CSafeQueue<CString>* jobQueue, CString& inputPath);
	~CProducer();
	void Stop();
	void Quit();
	void Start();

	UINT Run();
private:
	CSafeQueue<CString>* mJobQueue;
	CString mInputPath;
	BOOL mBQuitSignal;
	BOOL mBStopSignal;
};

