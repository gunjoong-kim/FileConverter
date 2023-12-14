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
#define MAX_THREAD_NUM 6

#define DLG_WIDTH 2000
#define DLG_HEIGHT 1500

#define THREADS_BOX_WIDTH 800
#define THREADS_BOX_HEIGHT 1400

#define VIEWER_BOX_WIDTH 1100
#define VIEWER_BOX_HEIGHT 1400

#define THREAD_BOX_WIDTH 700
#define THREAD_BOX_HEIGHT 200

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
	afx_msg LRESULT CConverter::OnUpdateThread(WPARAM wParam, LPARAM lParam);

public:
	UINT mThreadCnt;
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

private:
	CFont mTextFont;
	CFont mTitleFont;
	CFont mBtnFont;
	CStatic mGroupThread[6];
	CStatic mStatusThread[6];
	CStatic mSuccessThread[6];
	CStatic mFailureThread[6];
	CStatic mFNameThread[6];
	CStatic mGroupThreads;
	CButton mBtnInput;
	CButton mBtnOutput;
	CButton mBtnSave;
	CButton mBtnCancle;
	CButton mBtnFind;
	CButton mBtnRefresh;
	CStatic mGroupViewer;
	CListBox mListFile;
	CButton mBtnAdd;
	CEdit mEditContent;

public:
	afx_msg void OnBnClickedBtnFind();
	afx_msg void OnBnClickedBtnRefresh();
	afx_msg void OnClickedFile();
	afx_msg void OnStnClickedFailureThread(UINT nID);
	afx_msg void OnDropFiles(HDROP hDropInfo);
	afx_msg void OnBnClickedBtnAdd();
	afx_msg void OnBnClickedButtonInput();
	afx_msg void OnBnClickedButtonOutput();
	afx_msg void OnBnClickedButtonSave();
	afx_msg void OnBnClickedBtnModify();
	CButton mBtnModify;
};
