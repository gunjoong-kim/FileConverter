#include "pch.h"
#include "CProducer.h"

CProducer::CProducer(CSafeQueue<CString>* jobQueue, CString& inputPath)
	: mJobQueue(jobQueue)
	, mInputPath(inputPath)
	, mBQuitSignal(FALSE)
	, mBStopSignal(FALSE)
{}

CProducer::~CProducer() {}

UINT CProducer::Run()
{
	while (mBStopSignal == TRUE)
		Sleep(1000);
	if (mBQuitSignal == TRUE)
		return 0;
	CFileFind finder;
	CString searchPath = mInputPath + _T("\\*.*");
	BOOL bWorking = finder.FindFile(searchPath);
	while (bWorking)
	{
		bWorking = finder.FindNextFile();
		if (finder.IsDots() || finder.IsDirectory())
			continue;
		//ciritical section!!!
		mJobQueue->Enqueue(finder.GetFileName());
	}
	finder.Close();
	Sleep(1000);
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

//  while (!data->mQuitSignal)
//  {
	  //CFileFind finder;
	  //CString searchPath = data->mInputPath + _T("\\*.*");
	  //BOOL bWorking = finder.FindFile(searchPath);
	  //while (bWorking)
	  //{
	  //	bWorking = finder.FindNextFile();
	  //	if (finder.IsDots() || finder.IsDirectory())
	  //		continue;

	  //	//ciritical section!!!
	  //	data->mJobQueue.Enqueue(finder.GetFileName());
	  //}
	  //finder.Close();
	  //Sleep(1000);
//  }