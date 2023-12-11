
// FileConverterDlg.cpp: 구현 파일
//

#include "pch.h"
#include "framework.h"
#include "FileConverter.h"
#include "FileConverterDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 응용 프로그램 정보에 사용되는 CAboutDlg 대화 상자입니다.

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

// 구현입니다.
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CFileConverterDlg 대화 상자



CFileConverterDlg::CFileConverterDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_FILECONVERTER_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CFileConverterDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TEXT_INPUT, mTextInputPath);
	DDX_Control(pDX, IDC_TEXT_OUTPUT, mTextOutputPath);
	DDX_Control(pDX, IDC_TEXT_SAVE, mTextSavePath);
	DDX_Control(pDX, IDC_EDIT_INPUT, mEditInputPath);
	DDX_Control(pDX, IDC_EDIT_OUTPUT, mEditOutputPath);
	DDX_Control(pDX, IDC_EDIT_SAVE, mEditSavePath);
	DDX_Control(pDX, IDC_BUTTON_INPUT, mBtnInputFolder);
	DDX_Control(pDX, IDC_BUTTON_OUTPUT, mBtnOutputFolder);
	DDX_Control(pDX, IDC_BUTTON_SAVE, mBtnSaveFolder);
	DDX_Control(pDX, IDCANCEL, mBtnCancel);
	DDX_Control(pDX, IDC_TEXT_TITLE, mTextTitle);
	DDX_Control(pDX, IDOK, mBtnOk);
}

BEGIN_MESSAGE_MAP(CFileConverterDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_OUTPUT, &CFileConverterDlg::OnBnClickedButtonOutput)
	ON_BN_CLICKED(IDC_BUTTON_SAVE, &CFileConverterDlg::OnBnClickedButtonSave)
	ON_BN_CLICKED(IDC_BUTTON_INPUT, &CFileConverterDlg::OnBnClickedButtonInput)
	ON_BN_CLICKED(IDCANCEL, &CFileConverterDlg::OnBnClickedCancel)
	ON_BN_CLICKED(IDOK, &CFileConverterDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CFileConverterDlg 메시지 처리기

BOOL CFileConverterDlg::OnInitDialog()
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

	// 이 대화 상자의 아이콘을 설정합니다.  응용 프로그램의 주 창이 대화 상자가 아닐 경우에는
	//  프레임워크가 이 작업을 자동으로 수행합니다.
	SetIcon(m_hIcon, TRUE);			// 큰 아이콘을 설정합니다.
	SetIcon(m_hIcon, FALSE);		// 작은 아이콘을 설정합니다.

	// TODO: 여기에 추가 초기화 작업을 추가합니다.'
	LOGFONT logFont;
	mTextTitle.GetFont()->GetLogFont(&logFont);
	logFont.lfHeight = 150;
	mTitleFont.CreateFontIndirect(&logFont);
	mTextTitle.SetFont(&mTitleFont);
	mTextTitle.MoveWindow(130, 200, 900, 155);
;
	mTextInputPath.GetFont()->GetLogFont(&logFont);
	logFont.lfHeight = 40;
	mTextFont.CreateFontIndirectW(&logFont);

	mTextInputPath.SetFont(&mTextFont);
	mTextInputPath.MoveWindow(130, 400, 150, 40);

	mTextOutputPath.SetFont(&mTextFont);
	mTextOutputPath.MoveWindow(130, 450, 150, 40);

	mTextSavePath.SetFont(&mTextFont);
	mTextSavePath.MoveWindow(130, 500, 150, 40);

	mEditInputPath.MoveWindow(280, 400, 550, 45);
	mEditOutputPath.MoveWindow(280, 450, 550, 45);
	mEditSavePath.MoveWindow(280, 500, 550, 45);

	logFont.lfHeight = 30;
	mBtnFont.CreateFontIndirectW(&logFont);
	mBtnInputFolder.SetFont(&mBtnFont);
	mBtnInputFolder.MoveWindow(850, 400, 150, 45);
	mBtnOutputFolder.SetFont(&mBtnFont);
	mBtnOutputFolder.MoveWindow(850, 450, 150, 45);
	mBtnSaveFolder.SetFont(&mBtnFont);
	mBtnSaveFolder.MoveWindow(850, 500, 150, 45);

	return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
}

void CFileConverterDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 대화 상자에 최소화 단추를 추가할 경우 아이콘을 그리려면
//  아래 코드가 필요합니다.  문서/뷰 모델을 사용하는 MFC 애플리케이션의 경우에는
//  프레임워크에서 이 작업을 자동으로 수행합니다.

void CFileConverterDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 그리기를 위한 디바이스 컨텍스트입니다.

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 클라이언트 사각형에서 아이콘을 가운데에 맞춥니다.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 아이콘을 그립니다.
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// 사용자가 최소화된 창을 끄는 동안에 커서가 표시되도록 시스템에서
//  이 함수를 호출합니다.
HCURSOR CFileConverterDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CFileConverterDlg::OnBnClickedButtonOutput()
{
	CString outputInitPath = _T("C:\\");

	CFolderPickerDialog Picker(outputInitPath, OFN_FILEMUSTEXIST, NULL, 0);
	if (Picker.DoModal() == IDOK)
	{
		CString outputPath = Picker.GetPathName();
		mEditOutputPath.SetWindowTextW(outputPath);
	}
}


void CFileConverterDlg::OnBnClickedButtonSave()
{
	CString saveInitPath = _T("C:\\");

	CFolderPickerDialog Picker(saveInitPath, OFN_FILEMUSTEXIST, NULL, 0);
	if (Picker.DoModal() == IDOK)
	{
		CString savePath = Picker.GetPathName();
		mEditSavePath.SetWindowTextW(savePath);
	}
}


void CFileConverterDlg::OnBnClickedButtonInput()
{
	CString inputInitPath = _T("C:\\");

	CFolderPickerDialog Picker(inputInitPath, OFN_FILEMUSTEXIST, NULL, 0);
	if (Picker.DoModal() == IDOK)
	{
		CString inputPath = Picker.GetPathName();
		mEditInputPath.SetWindowTextW(inputPath);
	}
}


void CFileConverterDlg::OnBnClickedCancel()
{
	CDialogEx::OnCancel();
}

bool DoesFolderExist(const CString& folderPath)
{
	CFileFind finder;
	return finder.FindFile(folderPath) != 0 && finder.IsDirectory();
}

void CFileConverterDlg::OnBnClickedOk()
{
	// Debug Setting
	CString inputPath("C:\\Users\\김건중\\Documents\\INPUT\\");
	CString outputPath("C:\\Users\\김건중\\Documents\\OUTPUT\\");
	CString savePath("C:\\Users\\김건중\\Documents\\SAVE\\");
	//CString inputPath;
	//CString outputPath;
	//CString savePath;
	//GetDlgItem(IDC_EDIT_INPUT)->GetWindowText(inputPath);
	//GetDlgItem(IDC_EDIT_OUTPUT)->GetWindowText(outputPath);
	//GetDlgItem(IDC_EDIT_SAVE)->GetWindowTextW(savePath);

	/*if (DoesFolderExist(inputPath) == FALSE)
	{
		AfxMessageBox(_T("정확하지 않은 입력경로 입니다.")); 
		return;
	}
	if (DoesFolderExist(outputPath) == FALSE)
	{
		AfxMessageBox(_T("정확하지 않은 출력경로 입니다."));
		return;
	}
	if (DoesFolderExist(savePath) == FALSE)
	{
		AfxMessageBox(_T("정확하지 않은 보관경로 입니다."));
		return;
	}*/
	this->ShowWindow(SW_HIDE);
	CConverter converterWindow(nullptr, inputPath, outputPath, savePath);
	converterWindow.DoModal();
	this->ShowWindow(SW_SHOW);
}
