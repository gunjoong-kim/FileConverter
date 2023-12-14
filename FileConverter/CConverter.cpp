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
	, mThreadCnt(3)
{}

CConverter::~CConverter()
{
	delete mProducer;
	for (int i = 0; i < mConsumers.size(); i++)
		delete mConsumers[i];
}

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

	LOGFONT logFont;

	mGroupThreads.GetFont()->GetLogFont(&logFont);
	logFont.lfHeight = TITLE_FONT_SIZE;
	mTitleFont.CreateFontIndirect(&logFont);
	logFont.lfHeight = TEXT_FONT_SIZE;
	mTextFont.CreateFontIndirect(&logFont);
	logFont.lfHeight = BTN_FONT_SIZE;
	mBtnFont.CreateFontIndirect(&logFont);

	mGroupThreads.SetFont(&mTitleFont);
	mGroupViewer.SetFont(&mTitleFont);
	mListFile.SetFont(&mTextFont);
	mEditContent.SetFont(&mTextFont);

	mBtnInput.SetFont(&mBtnFont);
	mBtnOutput.SetFont(&mBtnFont);
	mBtnSave.SetFont(&mBtnFont);
	mBtnCancle.SetFont(&mBtnFont);
	mBtnFind.SetFont(&mBtnFont);
	mBtnRefresh.SetFont(&mBtnFont);
	mBtnAdd.SetFont(&mBtnFont);
	mBtnModify.SetFont(&mBtnFont);

	DragAcceptFiles(TRUE);
	SetWindowPos(&wndTop, 400, 300, DLG_WIDTH, DLG_HEIGHT, SWP_SHOWWINDOW);

	mGroupThreads.MoveWindow(20, 0, THREADS_BOX_WIDTH, THREADS_BOX_HEIGHT);
	mBtnAdd.MoveWindow(15 + THREADS_BOX_WIDTH - BTN_WIDTH, 30, BTN_WIDTH, BTN_HEIGHT);

	mGroupViewer.MoveWindow(THREADS_BOX_WIDTH + 40, 0, VIEWER_BOX_WIDTH, VIEWER_BOX_HEIGHT);
	mBtnFind.MoveWindow(THREADS_BOX_WIDTH + 50, 60, BTN_WIDTH, BTN_HEIGHT);
	mBtnRefresh.MoveWindow(THREADS_BOX_WIDTH + 50, 70 + BTN_HEIGHT, BTN_WIDTH, BTN_HEIGHT);
	mBtnModify.MoveWindow(THREADS_BOX_WIDTH + 50, 80 + BTN_HEIGHT * 2, BTN_WIDTH, BTN_HEIGHT);
	mListFile.MoveWindow(THREADS_BOX_WIDTH + 60 + BTN_WIDTH, 60, 900, 150);
	mEditContent.MoveWindow(THREADS_BOX_WIDTH + 50, 230, VIEWER_BOX_WIDTH - 40, 1100);
	mBtnInput.MoveWindow((BTN_WIDTH + 10) * 3 + THREADS_BOX_WIDTH, THREADS_BOX_HEIGHT - BTN_HEIGHT - 10, BTN_WIDTH, BTN_HEIGHT);
	mBtnOutput.MoveWindow((BTN_WIDTH + 10) * 4 + THREADS_BOX_WIDTH, THREADS_BOX_HEIGHT - BTN_HEIGHT - 10, BTN_WIDTH, BTN_HEIGHT);
	mBtnSave.MoveWindow((BTN_WIDTH + 10) * 5 + THREADS_BOX_WIDTH, THREADS_BOX_HEIGHT - BTN_HEIGHT - 10, BTN_WIDTH, BTN_HEIGHT);
	mBtnCancle.MoveWindow((BTN_WIDTH + 10) * 6 + THREADS_BOX_WIDTH, THREADS_BOX_HEIGHT - BTN_HEIGHT - 10, BTN_WIDTH, BTN_HEIGHT);

	for (int i = 0; i < MAX_THREAD_NUM; i++)
	{
		mGroupThread[i].MoveWindow(50, 60 + (25 + THREAD_BOX_HEIGHT) * i, THREAD_BOX_WIDTH, THREAD_BOX_HEIGHT);
		mFNameThread[i].MoveWindow(60, 60 + THREAD_BOX_HEIGHT / 2 + (25 + THREAD_BOX_HEIGHT) * i - STATIC_HEIGHT, STATIC_WIDTH * 3, STATIC_HEIGHT * 2);
		mStatusThread[i].MoveWindow(60, 60 + THREAD_BOX_HEIGHT / 2 + (25 + THREAD_BOX_HEIGHT) * i + STATIC_HEIGHT, STATIC_WIDTH, STATIC_HEIGHT);
		mSuccessThread[i].MoveWindow(280, 60 + THREAD_BOX_HEIGHT / 2 + (25 + THREAD_BOX_HEIGHT) * i + STATIC_HEIGHT, STATIC_WIDTH, STATIC_HEIGHT);
		mFailureThread[i].MoveWindow(500, 60 + THREAD_BOX_HEIGHT / 2 + (25 + THREAD_BOX_HEIGHT) * i + STATIC_HEIGHT, STATIC_WIDTH, STATIC_HEIGHT);
		mGroupThread[i].SetFont(&mTitleFont);
		mStatusThread[i].SetFont(&mTextFont);
		mSuccessThread[i].SetFont(&mTextFont);
		mFailureThread[i].SetFont(&mTextFont);
		mFNameThread[i].SetFont(&mTextFont);
	}

	mProducer = new CProducer(&mJobQueue, mInputDir);
	HWND hwnd = GetSafeHwnd();
	for (int i = 0; i < DEFAULT_THREAD_NUM; i++)
		mConsumers.push_back(new CBiConverter(i, &mJobQueue, mInputDir, mOutputDir, mSaveDir, hwnd, WM_UPDATE_THREAD1 + i));

	mProducerThread = AfxBeginThread(Produce, mProducer, THREAD_PRIORITY_NORMAL, 0, 0, NULL);
	for (int i = 0; i < DEFAULT_THREAD_NUM; i++)
		mConsumerThreads.push_back(AfxBeginThread(Consume, mConsumers[i], THREAD_PRIORITY_NORMAL, 0, 0, NULL));

	return TRUE;
}

void CConverter::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_GROUP_THREADS, mGroupThreads);
	DDX_Control(pDX, IDC_GROUP_THREAD1, mGroupThread[0]);
	DDX_Control(pDX, IDC_GROUP_THREAD2, mGroupThread[1]);
	DDX_Control(pDX, IDC_GROUP_THREAD3, mGroupThread[2]);
	DDX_Control(pDX, IDC_GROUP_THREAD4, mGroupThread[3]);
	DDX_Control(pDX, IDC_GROUP_THREAD5, mGroupThread[4]);
	DDX_Control(pDX, IDC_GROUP_THREAD6, mGroupThread[5]);
	DDX_Control(pDX, IDC_STATUS_THREAD1, mStatusThread[0]);
	DDX_Control(pDX, IDC_SUCCESS_THREAD1, mSuccessThread[0]);
	DDX_Control(pDX, IDC_FAILURE_THREAD1, mFailureThread[0]);
	DDX_Control(pDX, IDC_STATUS_THREAD2, mStatusThread[1]);
	DDX_Control(pDX, IDC_SUCCESS_THREAD2, mSuccessThread[1]);
	DDX_Control(pDX, IDC_FAILURE_THREAD2, mFailureThread[1]);
	DDX_Control(pDX, IDC_STATUS_THREAD3, mStatusThread[2]);
	DDX_Control(pDX, IDC_SUCCESS_THREAD3, mSuccessThread[2]);
	DDX_Control(pDX, IDC_FAILURE_THREAD3, mFailureThread[2]);
	DDX_Control(pDX, IDC_STATUS_THREAD4, mStatusThread[3]);
	DDX_Control(pDX, IDC_SUCCESS_THREAD4, mSuccessThread[3]);
	DDX_Control(pDX, IDC_FAILURE_THREAD4, mFailureThread[3]);
	DDX_Control(pDX, IDC_STATUS_THREAD5, mStatusThread[4]);
	DDX_Control(pDX, IDC_SUCCESS_THREAD5, mSuccessThread[4]);
	DDX_Control(pDX, IDC_FAILURE_THREAD5, mFailureThread[4]);
	DDX_Control(pDX, IDC_STATUS_THREAD6, mStatusThread[5]);
	DDX_Control(pDX, IDC_SUCCESS_THREAD6, mSuccessThread[5]);
	DDX_Control(pDX, IDC_FAILURE_THREAD6, mFailureThread[5]);
	DDX_Control(pDX, IDC_FNAME_THREAD1, mFNameThread[0]);
	DDX_Control(pDX, IDC_FNAME_THREAD2, mFNameThread[1]);
	DDX_Control(pDX, IDC_FNAME_THREAD3, mFNameThread[2]);
	DDX_Control(pDX, IDC_FNAME_THREAD4, mFNameThread[3]);
	DDX_Control(pDX, IDC_FNAME_THREAD5, mFNameThread[4]);
	DDX_Control(pDX, IDC_FNAME_THREAD6, mFNameThread[5]);
	DDX_Control(pDX, IDC_BUTTON_INPUT, mBtnInput);
	DDX_Control(pDX, IDC_BUTTON_OUTPUT, mBtnOutput);
	DDX_Control(pDX, IDC_BUTTON_SAVE, mBtnSave);
	DDX_Control(pDX, IDCANCEL, mBtnCancle);
	DDX_Control(pDX, IDC_BTN_FIND, mBtnFind);
	DDX_Control(pDX, IDC_BTN_REFRESH, mBtnRefresh);
	DDX_Control(pDX, IDC_GROUP_VIEWER, mGroupViewer);
	DDX_Control(pDX, IDC_LIST_FILE, mListFile);
	DDX_Control(pDX, IDC_EDIT_CONTENT, mEditContent);
	DDX_Control(pDX, IDC_BTN_ADD, mBtnAdd);
	DDX_Control(pDX, IDC_BTN_MODIFY, mBtnModify);
}

BEGIN_MESSAGE_MAP(CConverter, CDialogEx)
	ON_WM_DROPFILES()
	ON_BN_CLICKED(IDCANCEL, &CConverter::OnBnClickedCancel)
	ON_BN_CLICKED(IDSTOP, &CConverter::OnBnClickedStop)
	ON_MESSAGE(WM_UPDATE_THREAD1, &CConverter::OnUpdateThread)
	ON_MESSAGE(WM_UPDATE_THREAD2, &CConverter::OnUpdateThread)
	ON_MESSAGE(WM_UPDATE_THREAD3, &CConverter::OnUpdateThread)
	ON_MESSAGE(WM_UPDATE_THREAD4, &CConverter::OnUpdateThread)
	ON_MESSAGE(WM_UPDATE_THREAD5, &CConverter::OnUpdateThread)
	ON_MESSAGE(WM_UPDATE_THREAD6, &CConverter::OnUpdateThread)
	ON_BN_CLICKED(IDC_BTN_FIND, &CConverter::OnBnClickedBtnFind)
	ON_BN_CLICKED(IDC_BTN_REFRESH, &CConverter::OnBnClickedBtnRefresh)
	ON_LBN_SELCHANGE(IDC_LIST_FILE, &CConverter::OnClickedFile)
	ON_COMMAND_RANGE(IDC_FAILURE_THREAD1, IDC_FAILURE_THREAD6, OnStnClickedFailureThread)
	ON_BN_CLICKED(IDC_BTN_ADD, &CConverter::OnBnClickedBtnAdd)
	ON_BN_CLICKED(IDC_BUTTON_INPUT, &CConverter::OnBnClickedButtonInput)
	ON_BN_CLICKED(IDC_BUTTON_OUTPUT, &CConverter::OnBnClickedButtonOutput)
	ON_BN_CLICKED(IDC_BUTTON_SAVE, &CConverter::OnBnClickedButtonSave)
	ON_BN_CLICKED(IDC_BTN_MODIFY, &CConverter::OnBnClickedBtnModify)
END_MESSAGE_MAP()


// CConverter 메시지 처리기

void CConverter::OnDropFiles(HDROP hDropInfo)
{
	UINT nFiles = DragQueryFile(hDropInfo, 0xFFFFFFFF, NULL, 0);

	for (UINT i = 0; i < nFiles; ++i)
	{
		TCHAR szFilePath[MAX_PATH] = { 0 };
		DragQueryFile(hDropInfo, i, szFilePath, MAX_PATH);
		mJobQueue.Enqueue(szFilePath);
	}

	DragFinish(hDropInfo);

	CDialog::OnDropFiles(hDropInfo);
}

void CConverter::OnBnClickedCancel()
{
	for (int i = 0; i < mConsumerThreads.size(); i++)
		TerminateThread(mConsumerThreads[i], 0);
	TerminateThread(mProducerThread, 0);
	CDialogEx::OnCancel();
}

void CConverter::OnBnClickedStop()
{
	AfxMessageBox(mInputDir + mOutputDir + mSaveDir);
	mStopSignal = TRUE;
}

LRESULT CConverter::OnUpdateThread(WPARAM wParam, LPARAM lParam)
{
	threadInfo* info = reinterpret_cast<threadInfo*>(lParam);
	mStatusThread[info->idx].SetWindowText(info->status);
	CString num;
	num.Format(_T("%d"), info->success);
	mSuccessThread[info->idx].SetWindowText(_T("성공수량 : ") + num);
	num.Format(_T("%d"), info->failure);
	mFailureThread[info->idx].SetWindowText(_T("실패수량 : ") + num);
	mFNameThread[info->idx].SetWindowTextW(_T("파일이름 : ") + info->fileName);
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
		CString extension = GetExtension(filePath);
		if (extension == "atxt" || extension == "log")
			CViewer::ViewTxt(filePath, mEditContent);
		else if (extension == "abin")
			CViewer::ViewBin(filePath, mEditContent);
		else
			mEditContent.SetWindowTextW(_T("atxt, abin, log 파일을 열어주세요."));
	}
	else
		mEditContent.SetWindowText(_T("올바르지 않은 파일입니다."));
}

void CConverter::OnBnClickedBtnModify()
{
	int selectedIndex = mListFile.GetCurSel();
	if (selectedIndex != LB_ERR) {
		CString fileName;
		mListFile.GetText(selectedIndex, fileName);
		CString filePath = mViewerDir + _T("\\") + fileName;
		CString extension = GetExtension(filePath);
		if (extension == _T("atxt"))
		{
			CString content;
			mEditContent.GetWindowText(content);

			CStdioFile file;
			if (file.Open(filePath, CFile::modeReadWrite | CFile::typeBinary))
			{
				file.SeekToBegin();
				file.WriteString(content);
				file.Close();
			}
		}
		else
		{
			AfxMessageBox(_T("atxt 파일만 수정할 수 있습니다."));
		}
	}
}

void CConverter::OnStnClickedFailureThread(UINT nID)
{
	int index = nID - IDC_FAILURE_THREAD1;
	if (index >= mThreadCnt)
		return;
	ErrListDlg errListWindow(nullptr, &(mConsumers[index]->mErrList));
	errListWindow.DoModal();
}

void CConverter::OnBnClickedBtnAdd()
{
	if (mThreadCnt == MAX_THREAD_NUM)
	{
		AfxMessageBox(_T("더 이상 스레드를 추가할 수 없습니다."));
		return;
	}
	CString caption;
	caption.Format(_T("Thread%d : 활성화"), mThreadCnt + 1);
	mGroupThread[mThreadCnt].SetWindowText(caption);
	HWND hwnd = GetSafeHwnd();
	mConsumers.push_back(new CBiConverter(mThreadCnt, &mJobQueue, mInputDir, mOutputDir, mSaveDir, hwnd, WM_UPDATE_THREAD1 + mThreadCnt));
	mConsumerThreads.push_back(AfxBeginThread(Consume, mConsumers[mThreadCnt], THREAD_PRIORITY_NORMAL, 0, 0, NULL));
	mThreadCnt++;
}


void CConverter::OnBnClickedButtonInput()
{
	ShellExecute(NULL, _T("open"), mInputDir, NULL, NULL, SW_SHOWNORMAL);
}


void CConverter::OnBnClickedButtonOutput()
{
	ShellExecute(NULL, _T("open"), mOutputDir, NULL, NULL, SW_SHOWNORMAL);
}


void CConverter::OnBnClickedButtonSave()
{
	ShellExecute(NULL, _T("open"), mSaveDir, NULL, NULL, SW_SHOWNORMAL);
}
