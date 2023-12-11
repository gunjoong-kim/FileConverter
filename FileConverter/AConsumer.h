#pragma once

#include "CSafeQueue.h"

#define THREAD_WAIT_STR "상태 : 대기중"
#define THREAD_CONVERT_STR "상태 : 변환중"
#define THREAD_COMPLETE_STR "상태 : 완료"

#define OPEN_FAIL_ERR_MSG "파일을 여는데 실패하였습니다."
#define INVALID_HEADER_ERR_MSG "올바르지 않은 헤더입니다."
#define NO_CONTENT_ERR_MSG "파일에 내용물이 존재하지 않습니다."
#define CREATE_FAIL_ERR_MSG "파일을 생성하는데 실패하였습니다."

typedef struct converterInfo
{
	BOOL status;
	CString fullPath;
	CString errMsg;
	CTime startTime;
	CTime endTime;
} converterInfo;

typedef struct threadInfo
{
	CString status;
	int success;
	int failure;
} threadInfo;

class AConsumer
{
public:
	AConsumer(
		CSafeQueue<CString>* jobQueue,
		CString& inputDir,
		CString& outputDir,
		CString& saveDir,
		HWND hwnd,
		UINT handle
	);

	~AConsumer();
	void Stop();
	void Quit();
	void Start();
	UINT Run();
	void CreateLogFile(converterInfo data);
	virtual converterInfo Do(const CString& inputFullPath) = 0;
	BOOL CheckHeader(CString header, CString& outFileName);

public:
	std::vector<converterInfo> mErrList;

protected:
	BOOL mBStopSignal;
	BOOL mBQuitSignal;
	CSafeQueue<CString>* mJobQueue;
	CString mInputDir;
	CString mOutputDir;
	CString mSaveDir;
	HWND mHwnd;
	UINT mHandle;

private:
	AConsumer() {}
};

CString GetFileNameWithoutExtension(const CString& fullPath);
CString GetExtension(const CString& fullPath);
CString GetFileName(const CString& filePath);
CString GetFullPathWithNumbering(const CString& dirPath, const CString& fullPath);
