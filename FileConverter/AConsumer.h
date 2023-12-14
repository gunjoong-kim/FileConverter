#pragma once

#include "CSafeQueue.h"

#define THREAD_WAIT_STR "상태 : 대기중"
#define THREAD_CONVERT_STR "상태 : 변환중"
#define THREAD_COMPLETE_STR "상태 : 완료"

#define OPEN_FAIL_ERR_MSG "파일을 여는데 실패하였습니다."
#define INVALID_HEADER_ERR_MSG "올바르지 않은 헤더입니다."
#define NO_CONTENT_ERR_MSG "파일에 내용물이 존재하지 않습니다."
#define CREATE_FAIL_ERR_MSG "파일을 생성하는데 실패하였습니다."
#define INVALID_EXTENSION_ERR_MSG "올바르지 않은 확장자입니다."
#define INVALID_ENCODING_ERR_MSG "올바르지 않은 인코딩입니다."
#define INVALID_FILENAME_ERR_MSG "올바르지 않은 파일이름입니다."
#define FILE_SIZE_UNDER_18_ERR_MSG "파일 사이즈가 18바이트 미만입니다."
#define INVALID_OUTPUT_NAME_ERR_MSG "출력파일의 이름정의가 잘못되었습니다."

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
	CString fileName;
	UINT success;
	UINT failure;
	UINT idx;
} threadInfo;

class AConsumer
{
public:
	AConsumer(
		UINT threadIdx,
		CSafeQueue<CString>* jobQueue,
		CString& inputDir,
		CString& outputDir,
		CString& saveDir,
		HWND hwnd,
		UINT handle
	);

	~AConsumer();
	void Quit();
	UINT Run();
	void CreateLogFile(converterInfo data);
	virtual converterInfo Do(const CString& inputFullPath) = 0;
	BOOL CheckHeader(CString header, CString& outFileName);
	CString GetFullPathWithNumbering(const CString& dirPath, const CString& fullPath);

public:
	std::vector<converterInfo> mErrList;

protected:
	UINT mThreadIdx;
	BOOL mBStopSignal;
	BOOL mBQuitSignal;
	CSafeQueue<CString>* mJobQueue;
	CString mInputDir;
	CString mOutputDir;
	CString mSaveDir;
	HWND mHwnd;
	UINT mHandle;
	CRITICAL_SECTION mCS;
	CONDITION_VARIABLE mCV;

private:
	AConsumer() {}
};

CString GetFileNameWithoutExtension(const CString& fullPath);
CString GetExtension(const CString& fullPath);
CString GetFileName(const CString& filePath);
BOOL GetNextLine(CStdioFile& file, CString& line);
