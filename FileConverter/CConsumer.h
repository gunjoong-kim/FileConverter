#pragma once

#include "CSafeQueue.h"
#include "CBinToTxtConverter.h"
#include "CTxtToBinConverter.h"

#define THREAD_WAIT_STR "���� : �����"
#define THREAD_CONVERT_STR "���� : ��ȯ��"
#define THREAD_SUCCESS_STR "���� : ����"
#define THREAD_FAILURE_STR "���� : ����"

typedef struct threadInfo
{
	CString status;
	int success;
	int failure;
} threadInfo;

class CConsumer
{
public:
	CConsumer(
		CSafeQueue<CString>* jobQueue,
		CString& inputPath,
		CString& outputPath,
		CString& savePath,
		HWND hwnd,
		UINT handle
	);

	~CConsumer();
	void Stop();
	void Quit();
	void Start();
	UINT Run();

private:
	BOOL mBStopSignal;
	BOOL mBQuitSignal;
	CSafeQueue<CString>* mJobQueue;
	CString mInputPath;
	CString mOutputPath;
	CString mSavePath;
	HWND mHwnd;
	UINT mHandle;
};

