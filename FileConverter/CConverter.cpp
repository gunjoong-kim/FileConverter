#include "pch.h"
#include "FileConverter.h"
#include "afxdialogex.h"
#include "CConverter.h"

IMPLEMENT_DYNAMIC(CConverter, CDialogEx)


CConverter::CConverter(CWnd* pParent /*=nullptr*/, const CString& input, const CString& output, const CString& save)
	: CDialogEx(IDD_DIALOG_CONVERT, pParent)
	, mInputDir(input)
	, mOutputDir(output)
	, mSaveDir(save)
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
	AConsumer* consumer = reinterpret_cast<AConsumer*>(pParam);
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

	LOGFONT logFont;

	SetWindowPos(&wndTop, 400, 300, DLG_WIDTH, DLG_HEIGHT, SWP_SHOWWINDOW);

	//for (int i = 0; i < 3; i++)
	//{
	//	monitors[i] = new ThreadMonitor;
	//	monitors[i]->id = i;
	//	monitors[i]->Group = 
	//}



	mGroupThreads.MoveWindow(20, 0, THREADS_BOX_WIDTH, THREADS_BOX_HEIGHT);
	mBtnStart.MoveWindow(15 + THREADS_BOX_WIDTH - BTN_WIDTH, 30, BTN_WIDTH, BTN_HEIGHT);
	mBtnStop.MoveWindow(15 + THREADS_BOX_WIDTH - BTN_WIDTH * 2, 30, BTN_WIDTH, BTN_HEIGHT);

	mGroupViewer.MoveWindow(THREADS_BOX_WIDTH + 40, 0, VIEWER_BOX_WIDTH, VIEWER_BOX_HEIGHT);
	mBtnFind.MoveWindow(THREADS_BOX_WIDTH + 50, 60, BTN_WIDTH, BTN_HEIGHT);
	mBtnRefresh.MoveWindow(THREADS_BOX_WIDTH + 50, 70 + BTN_HEIGHT, BTN_WIDTH, BTN_HEIGHT);
	mListFile.MoveWindow(THREADS_BOX_WIDTH + 60 + BTN_WIDTH, 60, 900, 150);
	mEditContent.MoveWindow(THREADS_BOX_WIDTH + 50, 230, VIEWER_BOX_WIDTH - 40, 1050);

	mGroupThread1.MoveWindow(50, 60, THREAD_BOX_WIDTH, THREAD_BOX_HEIGHT);
	mStatusThread1.MoveWindow(60, 60 + THREAD_BOX_HEIGHT / 2, STATIC_WIDTH, STATIC_HEIGHT);
	mSuccessThread1.MoveWindow(280, 60 + THREAD_BOX_HEIGHT / 2, STATIC_WIDTH, STATIC_HEIGHT);
	mFailureThread1.MoveWindow(500, 60 + THREAD_BOX_HEIGHT / 2, STATIC_WIDTH, STATIC_HEIGHT);

	mGroupThread2.MoveWindow(50, 85 + THREAD_BOX_HEIGHT, THREAD_BOX_WIDTH, THREAD_BOX_HEIGHT);
	mStatusThread2.MoveWindow(60, 85 + THREAD_BOX_HEIGHT * 1.5, STATIC_WIDTH, STATIC_HEIGHT);
	mSuccessThread2.MoveWindow(280, 85 + THREAD_BOX_HEIGHT * 1.5, STATIC_WIDTH, STATIC_HEIGHT);
	mFailureThread2.MoveWindow(500, 85 + THREAD_BOX_HEIGHT * 1.5, STATIC_WIDTH, STATIC_HEIGHT);

	mGroupThread3.MoveWindow(50, 100 + THREAD_BOX_HEIGHT * 2, THREAD_BOX_WIDTH, THREAD_BOX_HEIGHT);
	mStatusThread3.MoveWindow(60, 100 + THREAD_BOX_HEIGHT * 2.5, STATIC_WIDTH, STATIC_HEIGHT);
	mSuccessThread3.MoveWindow(280, 100 + THREAD_BOX_HEIGHT * 2.5, STATIC_WIDTH, STATIC_HEIGHT);
	mFailureThread3.MoveWindow(500, 100 + THREAD_BOX_HEIGHT * 2.5, STATIC_WIDTH, STATIC_HEIGHT);
	
	mBtnInput.MoveWindow(DLG_WIDTH / 2, THREADS_BOX_HEIGHT - BTN_HEIGHT, BTN_WIDTH, BTN_HEIGHT);
	mBtnOutput.MoveWindow(BTN_WIDTH + 10 + DLG_WIDTH / 2, THREADS_BOX_HEIGHT - BTN_HEIGHT, BTN_WIDTH, BTN_HEIGHT);
	mBtnSave.MoveWindow((BTN_WIDTH + 10) * 2 + DLG_WIDTH / 2, THREADS_BOX_HEIGHT - BTN_HEIGHT, BTN_WIDTH, BTN_HEIGHT);
	mBtnCancle.MoveWindow((BTN_WIDTH + 10) * 3 + DLG_WIDTH / 2, THREADS_BOX_HEIGHT - BTN_HEIGHT, BTN_WIDTH, BTN_HEIGHT);

	mGroupThreads.GetFont()->GetLogFont(&logFont);
	logFont.lfHeight = TITLE_FONT_SIZE;
	mTitleFont.CreateFontIndirect(&logFont);
	mGroupThreads.SetFont(&mTitleFont);
	mGroupViewer.SetFont(&mTitleFont);
	mGroupThread1.SetFont(&mTitleFont);
	mGroupThread2.SetFont(&mTitleFont);
	mGroupThread3.SetFont(&mTitleFont);
	logFont.lfHeight = TEXT_FONT_SIZE;
	mTextFont.CreateFontIndirect(&logFont);
	mStatusThread1.SetFont(&mTextFont);
	mSuccessThread1.SetFont(&mTextFont);
	mFailureThread1.SetFont(&mTextFont);
	mStatusThread2.SetFont(&mTextFont);
	mSuccessThread2.SetFont(&mTextFont);
	mFailureThread2.SetFont(&mTextFont);
	mStatusThread3.SetFont(&mTextFont);
	mSuccessThread3.SetFont(&mTextFont);
	mFailureThread3.SetFont(&mTextFont);
	mListFile.SetFont(&mTextFont);
	mEditContent.SetFont(&mTextFont);
	logFont.lfHeight = BTN_FONT_SIZE;
	mBtnFont.CreateFontIndirect(&logFont);
	mBtnInput.SetFont(&mBtnFont);
	mBtnOutput.SetFont(&mBtnFont);
	mBtnSave.SetFont(&mBtnFont);
	mBtnStart.SetFont(&mBtnFont);
	mBtnStop.SetFont(&mBtnFont);
	mBtnCancle.SetFont(&mBtnFont);
	mBtnFind.SetFont(&mBtnFont);
	mBtnRefresh.SetFont(&mBtnFont);

	std::locale::global(std::locale("kor_KOR.949"));

	mProducer = new CProducer(&mJobQueue, mInputDir);
	HWND hwnd = GetSafeHwnd();
	for (int i = 0; i < DEFAULT_THREAD_NUM; i++)
		mConsumers.push_back(new CBiConverter(&mJobQueue, mInputDir, mOutputDir, mSaveDir, hwnd, WM_UPDATE_THREAD1 + i));

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
	DDX_Control(pDX, IDC_STATUS_THREAD1, mStatusThread1);
	DDX_Control(pDX, IDC_SUCCESS_THREAD1, mSuccessThread1);
	DDX_Control(pDX, IDC_FAILURE_THREAD1, mFailureThread1);
	DDX_Control(pDX, IDC_STATUS_THREAD2, mStatusThread2);
	DDX_Control(pDX, IDC_SUCCESS_THREAD2, mSuccessThread2);
	DDX_Control(pDX, IDC_FAILURE_THREAD2, mFailureThread2);
	DDX_Control(pDX, IDC_STATUS_THREAD3, mStatusThread3);
	DDX_Control(pDX, IDC_SUCCESS_THREAD3, mSuccessThread3);
	DDX_Control(pDX, IDC_FAILURE_THREAD3, mFailureThread3);
	DDX_Control(pDX, IDC_BUTTON_INPUT, mBtnInput);
	DDX_Control(pDX, IDC_BUTTON_OUTPUT, mBtnOutput);
	DDX_Control(pDX, IDC_BUTTON_SAVE, mBtnSave);
	DDX_Control(pDX, IDSTOP, mBtnStop);
	DDX_Control(pDX, IDCANCEL, mBtnCancle);
	DDX_Control(pDX, IDC_BTN_START, mBtnStart);
	DDX_Control(pDX, IDC_BTN_FIND, mBtnFind);
	DDX_Control(pDX, IDC_BTN_REFRESH, mBtnRefresh);
	DDX_Control(pDX, IDC_GROUP_VIEWER, mGroupViewer);
	DDX_Control(pDX, IDC_LIST_FILE, mListFile);
	DDX_Control(pDX, IDC_EDIT_CONTENT, mEditContent);
}


BEGIN_MESSAGE_MAP(CConverter, CDialogEx)
	ON_BN_CLICKED(IDCANCEL, &CConverter::OnBnClickedCancel)
	ON_BN_CLICKED(IDSTOP, &CConverter::OnBnClickedStop)
	ON_MESSAGE(WM_UPDATE_THREAD1, &CConverter::OnUpdateThread1)
	ON_MESSAGE(WM_UPDATE_THREAD2, &CConverter::OnUpdateThread2)
	ON_MESSAGE(WM_UPDATE_THREAD3, &CConverter::OnUpdateThread3)
	ON_BN_CLICKED(IDC_BTN_FIND, &CConverter::OnBnClickedBtnFind)
	ON_BN_CLICKED(IDC_BTN_REFRESH, &CConverter::OnBnClickedBtnRefresh)
	ON_LBN_SELCHANGE(IDC_LIST_FILE, &CConverter::OnClickedFile)
	ON_STN_CLICKED(IDC_FAILURE_THREAD1, &CConverter::OnStnClickedFailureThread1)
	ON_STN_CLICKED(IDC_FAILURE_THREAD2, &CConverter::OnStnClickedFailureThread2)
	ON_STN_CLICKED(IDC_FAILURE_THREAD3, &CConverter::OnStnClickedFailureThread3)
END_MESSAGE_MAP()


// CConverter 메시지 처리기

void CConverter::OnBnClickedCancel()
{
	mQuitSignal = TRUE;
	CDialogEx::OnCancel();
}

void CConverter::OnBnClickedStop()
{
	AfxMessageBox(mInputDir + mOutputDir + mSaveDir);
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
	mStatusThread3.SetWindowText(info->status);
	CString num;
	num.Format(_T("%d"), info->success);
	mSuccessThread3.SetWindowText(CString("성공수량 : ") + num);
	num.Format(_T("%d"), info->failure);
	mFailureThread3.SetWindowText(CString("실패수량 : ") + num);
	return 0;
}

void CConverter::OnBnClickedBtnFind()
{
	mListFile.ResetContent();
	CString initPath = _T("C:\\");

	CFolderPickerDialog Picker(initPath, OFN_FILEMUSTEXIST, NULL, 0);
	if (Picker.DoModal() == IDOK)
		mViewerDir = Picker.GetPathName();

	CFileFind finder;
	CString searchPath = mViewerDir + _T("\\*.*");
	BOOL bWorking = finder.FindFile(searchPath);
	HWND hwnd = GetSafeHwnd();
	while (bWorking)
	{
		bWorking = finder.FindNextFile();
		if (finder.IsDots() || finder.IsDirectory())
			continue;
		CString file = finder.GetFileName();
		mListFile.AddString(file);
	}
	finder.Close();
}

void CConverter::OnBnClickedBtnRefresh()
{
	mListFile.ResetContent();
	CFileFind finder;
	CString searchPath = mViewerDir + _T("\\*.*");
	BOOL bWorking = finder.FindFile(searchPath);
	HWND hwnd = GetSafeHwnd();
	while (bWorking)
	{
		bWorking = finder.FindNextFile();
		if (finder.IsDots() || finder.IsDirectory())
			continue;
		CString file = finder.GetFileName();
		mListFile.AddString(file);
	}
	finder.Close();
}

void CConverter::OnClickedFile()
{
	mEditContent.SetWindowText(_T(""));
	int selectedIndex = mListFile.GetCurSel();
	if (selectedIndex != LB_ERR) {
		CString fileName;
		mListFile.GetText(selectedIndex, fileName);
		CString filePath = mViewerDir + _T("\\") + fileName;
		CString extend = fileName.Right(5);
		if (extend == ".atxt")
			CViewer::ViewTxt(filePath, mEditContent);
		else if (extend == ".abin")
			CViewer::ViewBin(filePath, mEditContent);
		else
			mEditContent.SetWindowTextW(_T("atxt, abin 파일을 열어주세요."));
	}
	else
		mEditContent.SetWindowText(_T("올바르지 않은 파일입니다."));
}

void CConverter::OnStnClickedFailureThread1()
{
	ErrListDlg errListWindow(nullptr, &(mConsumers[0]->mErrList));
	errListWindow.DoModal();
}

void CConverter::OnStnClickedFailureThread2()
{
	ErrListDlg errListWindow(nullptr, &(mConsumers[1]->mErrList));
	errListWindow.DoModal();
}

void CConverter::OnStnClickedFailureThread3()
{
	ErrListDlg errListWindow(nullptr, &(mConsumers[2]->mErrList));
	errListWindow.DoModal();
}
