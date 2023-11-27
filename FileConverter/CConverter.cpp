// CConverter.cpp: 구현 파일
//

#include "pch.h"
#include "FileConverter.h"
#include "afxdialogex.h"
#include "CConverter.h"


// CConverter 대화 상자

IMPLEMENT_DYNAMIC(CConverter, CDialogEx)


CConverter::CConverter(CWnd* pParent /*=nullptr*/, const CString& input, const CString& output, const CString& save)
	: CDialogEx(IDD_DIALOG_CONVERT, pParent), inputPath(input), outputPath(output), savePath(save)
{
}

CConverter::~CConverter()
{
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
	AfxMessageBox(inputPath + outputPath + savePath);
}
