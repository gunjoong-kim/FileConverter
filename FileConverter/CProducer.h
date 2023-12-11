#pragma once
#include "CSafeQueue.h"

class CProducer
{
public:
	CProducer(CSafeQueue<CString>* jobQueue, CString& inputDir);
	~CProducer();
	void Stop();
	void Quit();
	void Start();

	UINT Run();
private:
	CSafeQueue<CString>* mJobQueue;
	CString mInputDir;
	BOOL mBQuitSignal;
	BOOL mBStopSignal;
};

