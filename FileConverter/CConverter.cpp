#include "pch.h"
#include "FileConverter.h"
#include "afxdialogex.h"
#include "CConverter.h"

IMPLEMENT_DYNAMIC(CConverter, CDialogEx)


CConverter::CConverter(CWnd* pParent /*=nullptr*/, const CString& input, const CString& output, const CString& save)
	: CDialogEx(IDD_DIALOG_CONVERT, pParent)
	, mInputPath(input)
	, mOutputPath(output)
	, mSavePath(save)
    , mStopSignal(false)
    , mQuitSignal(false)
{}

CConverter::~CConverter()
{}

UINT Produce(LPVOID pParam)
{
	CProducer* producer = (CProducer*)(pParam);
	producer->Run();
    return 0;
}

UINT Consume(LPVOID pParam)
{
	CConsumer* consumer = reinterpret_cast<CConsumer*>(pParam);
	consumer->Run();
    return 0;
}

BOOL CConverter::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 시스템 메뉴에 "정보..." 메뉴 항목을 추가합니다.

	// IDM_ABOUTBOX는 시스템 명령 범위에 있어야 합니다.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	SetWindowPos(&wndTop, 1000, 600, DLG_WIDTH, DLG_HEIGHT, SWP_SHOWWINDOW);

	mProducer = new CProducer(&mJobQueue, mInputPath);
	HWND hwnd = GetSafeHwnd();
	for (int i = 0; i < DEFAULT_THREAD_NUM; i++)
		mConsumers.push_back(new CConsumer(&mJobQueue, mInputPath, mOutputPath, mSavePath, hwnd, WM_UPDATE_THREAD1 + i));

	mProducerThread = AfxBeginThread(Produce, mProducer, THREAD_PRIORITY_NORMAL, 0, 0, NULL);
	for (int i = 0; i < DEFAULT_THREAD_NUM; i++)
		mConsumerThreads.push_back(AfxBeginThread(Consume, mConsumers[i], THREAD_PRIORITY_NORMAL, 0, 0, NULL));

	return TRUE;
}

void CConverter::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_GROUP_THREADS, mGroupThreads);
	DDX_Control(pDX, IDC_GROUP_THREAD1, mGroupThread1);
	DDX_Control(pDX, IDC_GROUP_THREAD2, mGroupThread2);
	DDX_Control(pDX, IDC_GROUP_THREAD3, mGroupThread3);
	DDX_Control(pDX, IDC_BUTTON_TADD, mBtnThreadAdd);
	DDX_Control(pDX, IDC_STATUS_THREAD1, mStatusThread1);
	DDX_Control(pDX, IDC_SUCCESS_THREAD1, mSuccessThread1);
	DDX_Control(pDX, IDC_FAILURE_THREAD1, mFailureThread1);
	DDX_Control(pDX, IDC_STATUS_THREAD2, mStatusThread2);
	DDX_Control(pDX, IDC_SUCCESS_THREAD2, mSuccessThread2);
	DDX_Control(pDX, IDC_FAILURE_THREAD2, mFailureThread2);
	DDX_Control(pDX, IDC_STATUS_THREAD3, mStatusThread3);
	DDX_Control(pDX, IDC_SUCCESS_THREAD3, mSuccessThread3);
	DDX_Control(pDX, IDC_FAILURE_THREAD3, mFailureThread3);
}


BEGIN_MESSAGE_MAP(CConverter, CDialogEx)
	ON_BN_CLICKED(IDCANCEL, &CConverter::OnBnClickedCancel)
	ON_BN_CLICKED(IDSTOP, &CConverter::OnBnClickedStop)
	ON_MESSAGE(WM_UPDATE_THREAD1, &CConverter::OnUpdateThread1)
	ON_MESSAGE(WM_UPDATE_THREAD2, &CConverter::OnUpdateThread2)
	ON_MESSAGE(WM_UPDATE_THREAD3, &CConverter::OnUpdateThread3)
END_MESSAGE_MAP()


// CConverter 메시지 처리기

void CConverter::OnBnClickedCancel()
{
	mQuitSignal = TRUE;
	CDialogEx::OnCancel();
}

void CConverter::OnBnClickedStop()
{
	AfxMessageBox(mInputPath + mOutputPath + mSavePath);
	mStopSignal = TRUE;
}


LRESULT CConverter::OnUpdateThread1(WPARAM wParam, LPARAM lParam)
{
	threadInfo* info = reinterpret_cast<threadInfo*>(lParam);
	mStatusThread1.SetWindowText(info->status);
	CString num;
	num.Format(_T("%d"), info->success);
	mSuccessThread1.SetWindowText(CString("성공수량 : ") + num);
	num.Format(_T("%d"), info->failure);
	mFailureThread1.SetWindowText(CString("실패수량 : ") + num);
	return 0;
}

LRESULT CConverter::OnUpdateThread2(WPARAM wParam, LPARAM lParam)
{
	threadInfo* info = reinterpret_cast<threadInfo*>(lParam);
	mStatusThread2.SetWindowText(info->status);
	CString num;
	num.Format(_T("%d"), info->success);
	mSuccessThread2.SetWindowText(CString("성공수량 : ") + num);
	num.Format(_T("%d"), info->failure);
	mFailureThread2.SetWindowText(CString("실패수량 : ") + num);
	return 0;
}
LRESULT CConverter::OnUpdateThread3(WPARAM wParam, LPARAM lParam)
{
	threadInfo* info = reinterpret_cast<threadInfo*>(lParam);
	mStatusThread1.SetWindowText(info->status);
	CString num;
	num.Format(_T("%d"), info->success);
	mSuccessThread3.SetWindowText(CString("성공수량 : ") + num);
	num.Format(_T("%d"), info->failure);
	mFailureThread3.SetWindowText(CString("실패수량 : ") + num);
	return 0;
}
