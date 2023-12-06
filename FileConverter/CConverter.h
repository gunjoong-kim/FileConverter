#pragma once

#include <queue>
#include "afxdialogex.h"
#include "CSafeQueue.h"
#include "CBinToTxtConverter.h"
#include "CTxtToBinConverter.h"
#include "CConsumer.h"
#include "CProducer.h"

#define DEFAULT_THREAD_NUM 3
#define MAX_THREAD_NUM 5

#define DLG_WIDTH 1200
#define DLG_HEIGHT 900

#define WM_UPDATE_THREAD1 (WM_USER + 1)
#define WM_UPDATE_THREAD2 (WM_USER + 2)
#define WM_UPDATE_THREAD3 (WM_USER + 3)

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
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedStop();
	afx_msg LRESULT OnUpdateThread1(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnUpdateThread2(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnUpdateThread3(WPARAM wParam, LPARAM lParam);

public:
	CString mInputPath;
	CString mOutputPath;
	CString mSavePath;
	CSafeQueue<CString> mJobQueue;
	CWinThread* mProducerThread;
	std::vector<CWinThread*> mConsumerThreads;
	CProducer* mProducer;
	std::vector<CConsumer*> mConsumers;
	BOOL mStopSignal;
	BOOL mQuitSignal;

private:
	CStatic mGroupThreads;
	CStatic mGroupThread1;
	CStatic mGroupThread2;
	CStatic mGroupThread3;
	CButton mBtnThreadAdd;
	CStatic mStatusThread1;
	CStatic mSuccessThread1;
	CStatic mFailureThread1;
	CStatic mStatusThread2;
	CStatic mSuccessThread2;
	CStatic mFailureThread2;
	CStatic mStatusThread3;
	CStatic mSuccessThread3;
	CStatic mFailureThread3;
};
