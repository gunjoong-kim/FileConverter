#pragma once

#include "CSafeQueue.h"
#include "CBinToTxtConverter.h"
#include "CTxtToBinConverter.h"

#define THREAD_WAIT_STR "상태 : 대기중"
#define THREAD_CONVERT_STR "상태 : 변환중"
#define THREAD_SUCCESS_STR "상태 : 성공"
#define THREAD_FAILURE_STR "상태 : 실패"

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

