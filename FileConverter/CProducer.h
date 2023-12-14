#pragma once
#include "CSafeQueue.h"

#define BUFFER_SIZE 1024 * 1024

class CProducer
{
public:
	CProducer(CSafeQueue<CString>* jobQueue, CString& inputDir);
	~CProducer();
	UINT Run();

private:
	BYTE* mBuffer;
	CSafeQueue<CString>* mJobQueue;
	CString mInputDir;
	CONDITION_VARIABLE* mCV;
};

