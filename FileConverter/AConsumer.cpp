#include "pch.h"
#include "AConsumer.h"

AConsumer::AConsumer(
	CSafeQueue<CString>* jobQueue,
	CString& inputDir,
	CString& outputDir,
	CString& saveDir,
	HWND hwnd,
	UINT handle
)
	: mJobQueue(jobQueue)
	, mInputDir(inputDir)
	, mOutputDir(outputDir)
	, mSaveDir(saveDir)
	, mHwnd(hwnd)
	, mHandle(handle)
{}

AConsumer::~AConsumer() {}

void AConsumer::Stop()
{
	mBStopSignal = TRUE;
}

void AConsumer::Quit()
{
	mBQuitSignal = TRUE;
}

void AConsumer::Start()
{
	mBStopSignal = FALSE;
}

// Function to check if a file exists
bool FileExists(const CString& fullPath)
{
	CFileStatus status;
	return CFile::GetStatus(fullPath, status);
}

BOOL AConsumer::CheckHeader(CString header, CString& outFileName)
{
	int tfnPos = header.Find(_T("[TargetFileName] "));
	if (tfnPos != 0)
		return FALSE;
	int spacePos = header.Find(_T(" "));
	if (spacePos != 16)
		return FALSE;
	outFileName = header.Mid(spacePos + 1);
	if (outFileName.FindOneOf(_T(" \t\n\r")) != -1)
		return FALSE;
	return TRUE;
}

CString GetFileNameWithoutExtension(const CString& fullPath)
{
	int lastBackslashPos = fullPath.ReverseFind(_T('\\'));
	int lastForwardSlashPos = fullPath.ReverseFind(_T('/'));
	int lastSeparatorPos = max(lastBackslashPos, lastForwardSlashPos);

	if (lastSeparatorPos != -1) {
		CString fileName = fullPath.Mid(lastSeparatorPos + 1);
		int lastDotPos = fileName.ReverseFind(_T('.'));

		if (lastDotPos != -1) {
			return fileName.Left(lastDotPos);
		}

		return fileName;
	}

	return fullPath;
}

CString GetExtension(const CString& fullPath)
{
	int lastDotPos = fullPath.ReverseFind(_T('.'));

	if (lastDotPos != -1) {
		return fullPath.Mid(lastDotPos + 1);
	}
	return fullPath;
}

CString GetFileName(const CString& filePath)
{
	int lastBackslashPos = filePath.ReverseFind(_T('\\'));
	int lastForwardSlashPos = filePath.ReverseFind(_T('/'));
	int lastSeparatorPos = max(lastBackslashPos, lastForwardSlashPos);

	if (lastSeparatorPos != -1)
	{
		CString fileName = filePath.Mid(lastSeparatorPos + 1);
		return fileName;
	}
	return filePath;
}

CString GetFullPathWithNumbering(const CString& dirPath, const CString& fullPath)
{
	// dirPath ended with slash
	CString fileName = GetFileNameWithoutExtension(fullPath);
	CString extension = GetExtension(fullPath);
	int num = 1;
	
	CString newFullPath = dirPath + fileName + _T(".") + extension;
	if (FileExists(newFullPath) == FALSE)
		return newFullPath;
	CString numbering;
	numbering.Format(_T("(%d)."), num);
	newFullPath = dirPath + fileName + numbering + extension;
	while (FileExists(newFullPath) == TRUE)
	{
		num++;
		numbering.Format(_T("(%d)."), num);
		newFullPath = dirPath + fileName + numbering + extension;
	}
	return newFullPath;
}

void AConsumer::CreateLogFile(converterInfo data)
{
	int number = 0;
	CString filePathBase = mSaveDir + _T("log_") + GetFileNameWithoutExtension(data.fullPath);
	CString logNumbering;
	logNumbering.Format(_T("_%d.atxt"), number);
	CString logFullPath = filePathBase + logNumbering;
	while (FileExists(logFullPath) == TRUE)
	{
		number++;
		logNumbering.Format(_T("_%d.atxt"), number);
		logFullPath = filePathBase + logNumbering;
	}
	CStdioFile logFile;
	if (logFile.Open(logFullPath, CFile::modeCreate | CFile::modeWrite | CFile::typeText)) {
		logFile.WriteString(_T("원본파일 경로 : ") + data.fullPath + _T("\n"));
		logFile.WriteString(_T("오류 내용 : ") + data.errMsg + _T("\n"));
		logFile.WriteString(_T("오류 일시 : ") + data.startTime.Format(_T("%Y-%m-%d %H:%M:%S")) + _T("\n"));
		logFile.Close();
	}
	else {
		AfxMessageBox(_T("ERROR: Can't Create Log File"));
	}
}

UINT AConsumer::Run()
{
	threadInfo msg;
	msg.status = THREAD_WAIT_STR;
	msg.success = 0;
	msg.failure = 0;
	converterInfo ret;
	while (!mBQuitSignal)
	{
		//대기중
		msg.status = THREAD_WAIT_STR;
		PostMessage(mHwnd, mHandle, 0, reinterpret_cast<LPARAM>(&msg));

		CString fullPath = mJobQueue->Dequeue();

		//변환 시작
		msg.status = THREAD_CONVERT_STR;
		PostMessage(mHwnd, mHandle, 0, reinterpret_cast<LPARAM>(&msg));
		ret = Do(fullPath);
		if (ret.status == TRUE)
			msg.success++;
		else if (ret.status == FALSE)
		{
			mErrList.push_back(ret);
			CreateLogFile(ret);
			msg.failure++;
		}
		CString newPath = mSaveDir + GetFileName(fullPath);
		CString newFullPath = GetFullPathWithNumbering(mSaveDir, fullPath);
		MoveFile(fullPath, newFullPath);

		//변환 완료
		msg.status = THREAD_COMPLETE_STR;
		PostMessage(mHwnd, mHandle, 0, reinterpret_cast<LPARAM>(&msg));
		Sleep(2000);
	}
	return 0;
}
