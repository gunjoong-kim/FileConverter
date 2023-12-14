#pragma once

#include "CSafeQueue.h"

#define THREAD_WAIT_STR "���� : �����"
#define THREAD_CONVERT_STR "���� : ��ȯ��"
#define THREAD_COMPLETE_STR "���� : �Ϸ�"

#define OPEN_FAIL_ERR_MSG "������ ���µ� �����Ͽ����ϴ�."
#define INVALID_HEADER_ERR_MSG "�ùٸ��� ���� ����Դϴ�."
#define NO_CONTENT_ERR_MSG "���Ͽ� ���빰�� �������� �ʽ��ϴ�."
#define CREATE_FAIL_ERR_MSG "������ �����ϴµ� �����Ͽ����ϴ�."
#define INVALID_EXTENSION_ERR_MSG "�ùٸ��� ���� Ȯ�����Դϴ�."
#define INVALID_ENCODING_ERR_MSG "�ùٸ��� ���� ���ڵ��Դϴ�."
#define INVALID_FILENAME_ERR_MSG "�ùٸ��� ���� �����̸��Դϴ�."
#define FILE_SIZE_UNDER_18_ERR_MSG "���� ����� 18����Ʈ �̸��Դϴ�."
#define INVALID_OUTPUT_NAME_ERR_MSG "��������� �̸����ǰ� �߸��Ǿ����ϴ�."

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
