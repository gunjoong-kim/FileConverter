#pragma once

#include <queue>
#include <locale>
#include "afxdialogex.h"
#include "CSafeQueue.h"
#include "CBiConverter.h"
#include "CProducer.h"
#include "CViewer.h"
#include "ErrListDlg.h"

#define DEFAULT_THREAD_NUM 3
#define MAX_THREAD_NUM 5

#define DLG_WIDTH 2000
#define DLG_HEIGHT 1500

#define THREADS_BOX_WIDTH 800
#define THREADS_BOX_HEIGHT 1400

#define VIEWER_BOX_WIDTH 1100
#define VIEWER_BOX_HEIGHT 1300

#define THREAD_BOX_WIDTH 700
#define THREAD_BOX_HEIGHT 180

#define STATIC_WIDTH 200
#define STATIC_HEIGHT 45

#define BTN_WIDTH 150
#define BTN_HEIGHT 45

#define TITLE_FONT_SIZE 40
#define TEXT_FONT_SIZE 30
#define BTN_FONT_SIZE 30

#define WM_UPDATE_CONTENT (WM_USER + 0)
#define WM_UPDATE_THREAD1 (WM_USER + 1)
#define WM_UPDATE_THREAD2 (WM_USER + 2)
#define WM_UPDATE_THREAD3 (WM_USER + 3)
#define WM_UPDATE_THREAD4 (WM_USER + 4)
#define WM_UPDATE_THREAD5 (WM_USER + 5)
#define WM_UPDATE_THREAD6 (WM_USER + 6)

#define THREAD_GROUP1_ID (WM_USER + 7)
#define THREAD_GROUP2_ID (WM_USER + 8)
#define THREAD_GROUP3_ID (WM_USER + 9)
#define THREAD_GROUP4_ID (WM_USER + 10)
#define THREAD_GROUP5_ID (WM_USER + 11)
#define THREAD_GROUP6_ID (WM_USER + 12)

#define THREAD_STATUS1_ID (WM_USER + 13)
#define THREAD_STATUS2_ID (WM_USER + 14)
#define THREAD_STATUS3_ID (WM_USER + 15)
#define THREAD_STATUS4_ID (WM_USER + 16)
#define THREAD_STATUS5_ID (WM_USER + 17)
#define THREAD_STATUS6_ID (WM_USER + 18)

#define THREAD_SUCCESS1_ID (WM_USER + 19)
#define THREAD_SUCCESS2_ID (WM_USER + 20)
#define THREAD_SUCCESS3_ID (WM_USER + 21)
#define THREAD_SUCCESS4_ID (WM_USER + 22)
#define THREAD_SUCCESS5_ID (WM_USER + 23)
#define THREAD_SUCCESS6_ID (WM_USER + 24)

#define THREAD_FAILURE1_ID (WM_USER + 25
#define THREAD_FAILURE2_ID (WM_USER + 26)
#define THREAD_FAILURE3_ID (WM_USER + 27)
#define THREAD_FAILURE4_ID (WM_USER + 28)
#define THREAD_FAILURE5_ID (WM_USER + 29)
#define THREAD_FAILURE6_ID (WM_USER + 30)



typedef struct ThreadMonitor
{
	CStatic Group;
	CStatic Status;
	CStatic Success;
	CStatic Failure;
	UINT id;
} ThreadMonitor;

class CConverter : public CDialogEx
{
	DECLARE_DYNAMIC(CConverter)

public:
	CConverter(CWnd* pParent /*= nullptr*/, const CString& input, const CString& output, const CString& save);
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
	CString mInputDir;
	CString mOutputDir;
	CString mSaveDir;
	CString mViewerDir;
	CSafeQueue<CString> mJobQueue;
	CWinThread* mProducerThread;
	std::vector<CWinThread*> mConsumerThreads;
	CProducer* mProducer;
	std::vector<AConsumer*> mConsumers;
	BOOL mStopSignal;
	BOOL mQuitSignal;

	std::vector<ThreadMonitor*> monitors;

private:
	CFont mTextFont;
	CFont mTitleFont;
	CFont mBtnFont;
	CStatic mGroupThreads;
	CStatic mGroupThread1;
	CStatic mGroupThread2;
	CStatic mGroupThread3;
	CStatic mStatusThread1;
	CStatic mSuccessThread1;
	CStatic mFailureThread1;
	CStatic mStatusThread2;
	CStatic mSuccessThread2;
	CStatic mFailureThread2;
	CStatic mStatusThread3;
	CStatic mSuccessThread3;
	CStatic mFailureThread3;
	CButton mBtnInput;
	CButton mBtnOutput;
	CButton mBtnSave;
	CButton mBtnStop;
	CButton mBtnCancle;
	CButton mBtnStart;
	CButton mBtnFind;
	CButton mBtnRefresh;
	CStatic mGroupViewer;
	CListBox mListFile;
public:
	afx_msg void OnBnClickedBtnFind();
	afx_msg void OnBnClickedBtnRefresh();
	afx_msg void OnClickedFile();
	CEdit mEditContent;
	afx_msg void OnStnClickedFailureThread1();
	afx_msg void OnStnClickedFailureThread2();
	afx_msg void OnStnClickedFailureThread3();
};
