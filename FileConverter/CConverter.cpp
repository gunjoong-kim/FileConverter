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
{
	mPParam.inputPath = mInputPath;
	mPParam.bQuitSignal = FALSE;
	mPParam.bStopSignal = FALSE;
	mPParam.jobQueue = &mJobQueue;
	for (int i = 0; i < DEFAULT_THREAD_NUM; i++)
	{
		conParam tmp;
		tmp.inputPath = mInputPath;
		tmp.outputPath = mOutputPath;
		tmp.savePath = mSavePath;
		tmp.handle = WM_UPDATE_THREAD1 + i;
		tmp.bQuitSignal = FALSE;
		tmp.bStopSignal = FALSE;
		tmp.jobQueue = &mJobQueue;
		mCParamVector.push_back(tmp);
	}
}

CConverter::~CConverter()
{		

}

BOOL CheckHeader(CString header, CString& outFileName)
{
	//조금 더 다듬어야 함
	int tfnPos = header.Find(_T("[TargetFileName] "));
	if (tfnPos != 0)
		return FALSE;
	int spacePos = header.Find(_T(" "));
	if (spacePos != 16)
		return FALSE;
	outFileName = header.Mid(17);
	if (outFileName.FindOneOf(_T(" \t\n\r")) != -1)
		return FALSE;
	return TRUE;
}

BOOL ConvertTxtToBin(CString outputPath, CString filePath)
{
	CStdioFile textFile;
	if (!textFile.Open(filePath, CFile::modeRead | CFile::typeText))
	{
		AfxMessageBox(CString("Failed to open the text file. : ") + filePath);
		return FALSE;
	}

	CString header;
	if (!textFile.ReadString(header))
	{
		AfxMessageBox(CString("No Header : ") + filePath);
		return FALSE;
	}

	CString fileName;
	if (CheckHeader(header, fileName) == FALSE)
	{
		AfxMessageBox(CString("Invalid Header : ") + filePath);
		return FALSE;
	}

	CFile binaryFile;
	if (!binaryFile.Open(outputPath + fileName + _T(".abin"), CFile::modeCreate | CFile::modeWrite | CFile::typeBinary))
	{
		textFile.Close();
		AfxMessageBox(_T("Failed to create the binary file."));
		return FALSE;
	}

	CString line;
	header += NEWLINE;
	binaryFile.Write(header.GetString(), header.GetLength() * sizeof(TCHAR));
	while (textFile.ReadString(line))
	{
		line += NEWLINE;
		binaryFile.Write(line.GetString(), line.GetLength() * sizeof(TCHAR));
	}
	textFile.Close();
	binaryFile.Close();

	return TRUE;
}

BOOL ConvertBinToTxt(CString outputPath, CString filePath)
{
	CFile inputFile;
	if (!inputFile.Open(filePath, CFile::modeRead | CFile::typeBinary))
	{
		AfxMessageBox(CString("Failed to open the bin file. : ") + filePath);
		return FALSE;
	}

	CString header;
	while (true)
	{
		TCHAR currentChar;
		if (inputFile.Read(&currentChar, sizeof(currentChar)) != sizeof(currentChar))
		{
			AfxMessageBox(CString("No Header : ") + filePath);
			return FALSE;
		}
		if (currentChar == '\n')
			break;
		if (currentChar == '\r')
			continue;
		header += currentChar;
	}
	CString fileName;
	if (CheckHeader(header, fileName) == FALSE)
	{
		AfxMessageBox(CString("Invalid Header : ") + filePath);
		return FALSE;
	}

	// 텍스트 파일 생성
	CStdioFile outputFile;
	if (!outputFile.Open(outputPath + fileName + _T(".atxt"), CFile::modeCreate | CFile::modeWrite | CFile::typeText))
	{
		AfxMessageBox(_T("Failed to open output text file"));
		inputFile.Close();
		return FALSE;
	}

	// 파일 크기 확인
	inputFile.SeekToBegin();
	UINT fileSize = (UINT)inputFile.GetLength() * 2;

	// 바이너리 파일에서 데이터 읽어오기
	CString str;
	TCHAR* buffer = new TCHAR[fileSize / sizeof(TCHAR)];
	inputFile.Read(buffer, fileSize);
	str.SetString(buffer, fileSize / sizeof(TCHAR));
	outputFile.WriteString(str);

	delete[] buffer;
	inputFile.Close();
	outputFile.Close();

	return TRUE;
}


UINT Produce(LPVOID pParam)
{
	proParam* data = reinterpret_cast<proParam*>(pParam);
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
	CFileFind finder;
	CString searchPath = data->inputPath + _T("\\*.*");
	BOOL bWorking = finder.FindFile(searchPath);
	while (bWorking)
	{
		bWorking = finder.FindNextFile();
		if (finder.IsDots() || finder.IsDirectory())
			continue;
		//ciritical section!!!
		data->jobQueue->Enqueue(finder.GetFileName());
	}
	finder.Close();
	Sleep(1000);
    return 0;
}

UINT Consume(LPVOID pParam)
{
	threadInfo msg;
	msg.status = THREAD_WAIT_STR;
	msg.success = 0;
	msg.failure = 0;
	conParam* data = reinterpret_cast<conParam*>(pParam);
	DWORD threadID = GetCurrentThreadId();
	BOOL ret;
	// ID를 문자열로 변환
	CString threadIDString;
	threadIDString.Format(_T("%lu"), threadID);
	msg.status = THREAD_WAIT_STR;
	PostMessage(data->hWnd, data->handle, 0, reinterpret_cast<LPARAM>(&msg));
	while (!data->bQuitSignal)
	{
		CString fileName = data->jobQueue->Dequeue();
		CString filePath = data->inputPath + fileName;

		msg.status = THREAD_CONVERT_STR;
		PostMessage(data->hWnd, data->handle, 0, reinterpret_cast<LPARAM>(&msg));
		if (fileName.Right(5) == _T(".abin"))
			ret = ConvertBinToTxt(data->outputPath, filePath);
		else if (fileName.Right(5) == _T(".atxt"))
			ret = ConvertTxtToBin(data->outputPath, filePath);
		if (ret == TRUE)
		{
			msg.status = THREAD_SUCCESS_STR;
			msg.success++;
		}
		else
		{
			msg.status = THREAD_FAILURE_STR;
			msg.failure++;
		}
		PostMessage(data->hWnd, data->handle, 0, reinterpret_cast<LPARAM>(&msg));
		Sleep(2000);
		msg.status = THREAD_WAIT_STR;
		PostMessage(data->hWnd, data->handle, 0, reinterpret_cast<LPARAM>(&msg));
	}
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
	mProducer = AfxBeginThread(Produce, &mPParam, THREAD_PRIORITY_NORMAL, 0, 0, NULL);
	for (int i = 0; i < DEFAULT_THREAD_NUM; i++)
	{
		mCParamVector[i].hWnd = GetSafeHwnd();
		mConsumerVector.push_back(AfxBeginThread(Consume, &mCParamVector[i], THREAD_PRIORITY_NORMAL, 0, 0, NULL));
	}

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
