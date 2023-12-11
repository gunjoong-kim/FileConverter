#include "pch.h"
#include "CProducer.h"

CProducer::CProducer(CSafeQueue<CString>* jobQueue, CString& inputDir)
	: mJobQueue(jobQueue)
	, mInputDir(inputDir)
	, mBQuitSignal(FALSE)
	, mBStopSignal(FALSE)
{}

CProducer::~CProducer() {}

UINT CProducer::Run()
{
	//기존 파일 추가코드

	HANDLE hDir = CreateFileW(mInputDir, GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE,
		0, OPEN_EXISTING, FILE_FLAG_BACKUP_SEMANTICS, 0);
	if (hDir == INVALID_HANDLE_VALUE) {
		DWORD dwLastError = GetLastError();
		CString errorMessage;
		errorMessage.Format(_T("Failed to open directory '%s'. Error %d."), mInputDir, dwLastError);
		AfxMessageBox(errorMessage);
		return 1;
	}

	CONST DWORD bufferSize = 1024 * 1024;
	BYTE* buffer = (PBYTE)malloc(bufferSize);
	BOOL bWatchSubtree = FALSE;
	DWORD dwNotifyFilter =
		FILE_NOTIFY_CHANGE_FILE_NAME | FILE_NOTIFY_CHANGE_DIR_NAME |
		FILE_NOTIFY_CHANGE_ATTRIBUTES | FILE_NOTIFY_CHANGE_SIZE |
		FILE_NOTIFY_CHANGE_LAST_WRITE | FILE_NOTIFY_CHANGE_CREATION;
	DWORD bytesReturned;
	WCHAR temp[MAX_PATH] = { 0 };
    while (1)
    {
        FILE_NOTIFY_INFORMATION* pfni;
        BOOL fOk = ReadDirectoryChangesW(hDir, buffer, bufferSize, FALSE, dwNotifyFilter, &bytesReturned, NULL, NULL);
        if (!fOk)
        {
            DWORD dwLastError = GetLastError();
			AfxMessageBox(_T("ERROR : ") + dwLastError);
			delete[] buffer;
			return 1;
        }

        pfni = (FILE_NOTIFY_INFORMATION*)buffer;

		Sleep(1000);
        do
		{
			CString fullPath = mInputDir;
			fullPath.Append(pfni->FileName, pfni->FileNameLength / sizeof(WCHAR));
			if (pfni->Action == FILE_ACTION_ADDED)
				mJobQueue->Enqueue(fullPath);
            pfni = (FILE_NOTIFY_INFORMATION*)((PBYTE)pfni + pfni->NextEntryOffset);
        } while (pfni->NextEntryOffset > 0);
    }
	delete[] buffer;
	return 0;
}

void CProducer::Stop()
{
	mBStopSignal = TRUE;
}

void CProducer::Quit()
{
	mBQuitSignal = TRUE;
}

void CProducer::Start()
{
	mBStopSignal = FALSE;
}
