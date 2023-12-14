#include "pch.h"
#include "AConsumer.h"

AConsumer::AConsumer(
	UINT threadIdx,
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
	, mThreadIdx(threadIdx)
{
	InitializeCriticalSection(&mCS);
}

AConsumer::~AConsumer() {}

void AConsumer::Quit()
{
	mBQuitSignal = TRUE;
}

bool FileExists(const CString& fullPath)
{
	CFileStatus status;
	return CFile::GetStatus(fullPath, status);
}

BOOL GetNextLine(CStdioFile& file, CString& line)
{
	CString ret;
	wchar_t buffer;
	UINT readRet;
	while (1)
	{
		readRet = file.Read(&buffer, sizeof(wchar_t));
		if (readRet != sizeof(wchar_t))
		{
			line = ret;
			return FALSE;
		}
		if (buffer == _T('\r'))
			continue;
		if (buffer == _T('\n'))
		{
			line = ret;
			return TRUE;
		}
		ret += buffer;
	}
	line = ret;
	return FALSE;
}

BOOL AConsumer::CheckHeader(CString header, CString& outFileName)
{
	int tfnPos = header.Find(_T("[ATRANS] "));
	if (tfnPos != 0)
		return FALSE;
	int spacePos = header.Find(_T(" "));
	if (spacePos != 8)
		return FALSE;
	outFileName = header.Mid(spacePos + 1);
	if (outFileName.GetLength() == 0)
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

CString AConsumer::GetFullPathWithNumbering(const CString& dirPath, const CString& fullPath)
{
	CString fileName = GetFileNameWithoutExtension(fullPath);
	CString extension = GetExtension(fullPath);
	int num = 1;
	
	// 파일을 열때까지 상호배제가 유지되어야 함
	EnterCriticalSection(&mCS);
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
	LeaveCriticalSection(&mCS);
	return newFullPath;
}

void AConsumer::CreateLogFile(converterInfo data)
{
	int number = 0;
	char BOM[2];
	BOM[0] = (char)0xFF;
	BOM[1] = (char)0xFE;
	CString logFullPath = mSaveDir + GetFileNameWithoutExtension(data.fullPath) 
		+ _T("_") + GetExtension(data.fullPath) + _T(".log");
	logFullPath = GetFullPathWithNumbering(mSaveDir, logFullPath);
	CStdioFile logFile;
	if (logFile.Open(logFullPath, CFile::modeCreate | CFile::modeWrite | CFile::typeBinary)) {
		logFile.Write(BOM, 2);
		logFile.WriteString(_T("원본파일 경로 : ") + data.fullPath + _T("\r\n"));
		logFile.WriteString(_T("오류 내용 : ") + data.errMsg + _T("\r\n"));
		logFile.WriteString(_T("오류 일시 : ") + data.startTime.Format(_T("%Y-%m-%d %H:%M:%S")) + _T("\r\n"));
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
	msg.idx = mThreadIdx;
	converterInfo ret;
	while (!mBQuitSignal)
	{
		//대기중
		msg.status = THREAD_WAIT_STR;
		PostMessage(mHwnd, mHandle, 0, reinterpret_cast<LPARAM>(&msg));

		CString fullPath = mJobQueue->Dequeue();

		//변환 시작
		msg.fileName = GetFileName(fullPath);
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
		CString saveDir = mSaveDir;
		CString newPath = saveDir + GetFileName(fullPath);
		CString newFullPath = GetFullPathWithNumbering(saveDir, fullPath);
		MoveFile(fullPath, newFullPath);

		//변환 완료
		msg.status = THREAD_COMPLETE_STR;
		PostMessage(mHwnd, mHandle, 0, reinterpret_cast<LPARAM>(&msg));
		Sleep(2000);
	}
	return 0;
}