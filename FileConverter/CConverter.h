#pragma once

#include <queue>
#include "afxdialogex.h"
#include "CSafeQueue.h"

#define DEFAULT_THREAD_NUM 3
#define MAX_THREAD_NUM 5
#define NEWLINE "\n"

class CConverter : public CDialogEx
{
	DECLARE_DYNAMIC(CConverter)

public:
	CConverter(CWnd* pParent /*=nullptr*/, const CString& input, const CString& output, const CString& save);
	virtual ~CConverter();

#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_CONVERT };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnBnClickedOk();

public:
	CString mInputPath;
	CString mOutputPath;
	CString mSavePath;
	CSafeQueue<CString> mJobQueue;
	CWinThread* mProducer;
	std::vector<CWinThread*> mConsumerVector;
	BOOL mStopSignal;
	BOOL mQuitSignal;
};
