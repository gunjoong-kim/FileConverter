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
    CConverter* data = reinterpret_cast<CConverter*>(pParam);

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
	CString searchPath = data->mInputPath + _T("\\*.*");
	BOOL bWorking = finder.FindFile(searchPath);
	while (bWorking)
	{
		bWorking = finder.FindNextFile();
		if (finder.IsDots() || finder.IsDirectory())
			continue;
		//ciritical section!!!
		data->mJobQueue.Enqueue(finder.GetFileName());
	}
	finder.Close();
	Sleep(1000);
    return 0;
}

UINT Consume(LPVOID pParam)
{
	CConverter* data = reinterpret_cast<CConverter*>(pParam);

	DWORD threadID = GetCurrentThreadId();

	// ID를 문자열로 변환
	CString threadIDString;
	threadIDString.Format(_T("%lu"), threadID);

	while (!data->mQuitSignal)
	{
		//대기 UI
		CString fileName = data->mJobQueue.Dequeue();
		CString filePath = data->mInputPath + fileName;
		//AfxMessageBox(CString("변환 시작 : ") + fileName);
		if (fileName.Right(5) == _T(".abin"))
			ConvertBinToTxt(data->mOutputPath, filePath);
		else if (fileName.Right(5) == _T(".atxt"))
			ConvertTxtToBin(data->mOutputPath, filePath);
		else
			continue;
		//AfxMessageBox(CString("변환 완료 : ") + fileName);
		//Sleep(2000);
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

	mProducer = AfxBeginThread(Produce, this, THREAD_PRIORITY_NORMAL, 0, 0, NULL);
	for (int i = 0; i < DEFAULT_THREAD_NUM; i++)
		mConsumerVector.push_back(AfxBeginThread(Consume, this, THREAD_PRIORITY_NORMAL, 0, 0, NULL));

    return TRUE;
}

void CConverter::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CConverter, CDialogEx)
	ON_BN_CLICKED(IDOK, &CConverter::OnBnClickedOk)
END_MESSAGE_MAP()


// CConverter 메시지 처리기


void CConverter::OnBnClickedOk()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	AfxMessageBox(mInputPath + mOutputPath + mSavePath);
}
