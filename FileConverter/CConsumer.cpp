#include "pch.h"
#include "CConsumer.h"

CConsumer::CConsumer(
	CSafeQueue<CString>* jobQueue,
	CString& inputPath,
	CString& outputPath,
	CString& savePath,
	HWND hwnd,
	UINT handle
)
	: mJobQueue(jobQueue)
	, mInputPath(inputPath)
	, mOutputPath(outputPath)
	, mSavePath(savePath)
	, mHwnd(hwnd)
	, mHandle(handle)
{}

CConsumer::~CConsumer() {}

void CConsumer::Stop()
{
	mBStopSignal = TRUE;
}

void CConsumer::Quit()
{
	mBQuitSignal = TRUE;
}

void CConsumer::Start()
{
	mBStopSignal = FALSE;
}

UINT CConsumer::Run()
{
	threadInfo msg;
	msg.status = THREAD_WAIT_STR;
	msg.success = 0;
	msg.failure = 0;
	DWORD threadID = GetCurrentThreadId();
	BOOL ret;
	CString threadIDString;
	threadIDString.Format(_T("%lu"), threadID);
	msg.status = THREAD_WAIT_STR;
	PostMessage(mHwnd, mHandle, 0, reinterpret_cast<LPARAM>(&msg));
	while (!mBQuitSignal)
	{
		CString fileName = mJobQueue->Dequeue();
		CString filePath = mInputPath + fileName;

		msg.status = THREAD_CONVERT_STR;
		PostMessage(mHwnd, mHandle, 0, reinterpret_cast<LPARAM>(&msg));
		if (fileName.Right(5) == _T(".abin"))
			ret = CBinToTxtConverter::convert(mOutputPath, filePath);
		else if (fileName.Right(5) == _T(".atxt"))
			ret = CTxtToBinConverter::convert(mOutputPath, filePath);
		if (ret == TRUE)
		{
			msg.status = THREAD_SUCCESS_STR;
			msg.success++;
		}
		else
		{
			msg.status = THREAD_FAILURE_STR;
			msg.failure++;
		}
		PostMessage(mHwnd, mHandle, 0, reinterpret_cast<LPARAM>(&msg));
		Sleep(2000);
		msg.status = THREAD_WAIT_STR;
		PostMessage(mHwnd, mHandle, 0, reinterpret_cast<LPARAM>(&msg));
	}
	return 0;
}