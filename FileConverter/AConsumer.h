#pragma once

#include "CSafeQueue.h"

#define THREAD_WAIT_STR "���� : �����"
#define THREAD_CONVERT_STR "���� : ��ȯ��"
#define THREAD_COMPLETE_STR "���� : �Ϸ�"

#define OPEN_FAIL_ERR_MSG "������ ���µ� �����Ͽ����ϴ�."
#define INVALID_HEADER_ERR_MSG "�ùٸ��� ���� ����Դϴ�."
#define NO_CONTENT_ERR_MSG "���Ͽ� ���빰�� �������� �ʽ��ϴ�."
#define CREATE_FAIL_ERR_MSG "������ �����ϴµ� �����Ͽ����ϴ�."

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
