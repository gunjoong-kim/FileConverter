// ErrListDlg.cpp: 구현 파일
//

#include "pch.h"
#include "FileConverter.h"
#include "afxdialogex.h"
#include "ErrListDlg.h"


// ErrListDlg 대화 상자

IMPLEMENT_DYNAMIC(ErrListDlg, CDialogEx)

ErrListDlg::ErrListDlg(CWnd* pParent /*=nullptr*/, std::vector<converterInfo>* errList)
	: CDialogEx(IDD_DIALOG1, pParent)
	, mErrList(errList)
{}

ErrListDlg::~ErrListDlg()
{
}

void ErrListDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_ERR, mListErr);
}

BOOL ErrListDlg::OnInitDialog()
{
    CDialogEx::OnInitDialog();

    mListErr.SetExtendedStyle(
        LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT);

    mListErr.InsertColumn(0, _T("파일 경로"), LVCFMT_CENTER, 700);
    mListErr.InsertColumn(1, _T("수행 일시"), LVCFMT_CENTER, 300);
    mListErr.InsertColumn(2, _T("오류 내용"), LVCFMT_CENTER, 500);

    for (int i = 0; i < mErrList->size(); i++)
    {
        mListErr.InsertItem(i, (*mErrList)[i].fullPath);
        mListErr.SetItemText(i, 1, (*mErrList)[i].startTime.Format(_T("%Y-%m-%d %H:%M:%S")));
        mListErr.SetItemText(i, 2, (*mErrList)[i].errMsg);
    }
    return TRUE;
}

BEGIN_MESSAGE_MAP(ErrListDlg, CDialogEx)

    ON_BN_CLICKED(IDREFRESH, &ErrListDlg::OnBnClickedRefresh)
    ON_BN_CLICKED(IDCLOSE, &ErrListDlg::OnBnClickedClose)
END_MESSAGE_MAP()


void ErrListDlg::OnBnClickedRefresh()
{
    // TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
    mListErr.DeleteAllItems();
    mListErr.InsertColumn(0, _T("파일 경로"), LVCFMT_CENTER, 700);
    mListErr.InsertColumn(1, _T("수행 일시"), LVCFMT_CENTER, 300);
    mListErr.InsertColumn(2, _T("오류 내용"), LVCFMT_CENTER, 500);

    for (int i = 0; i < mErrList->size(); i++)
    {
        mListErr.InsertItem(i, (*mErrList)[i].fullPath);
        mListErr.SetItemText(i, 1, (*mErrList)[i].startTime.Format(_T("%Y-%m-%d %H:%M:%S")));
        mListErr.SetItemText(i, 2, (*mErrList)[i].errMsg);
    }
}


void ErrListDlg::OnBnClickedClose()
{
    CDialogEx::OnCancel();
}
