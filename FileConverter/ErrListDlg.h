#pragma once
#include "afxdialogex.h"
#include "AConsumer.h";

// ErrListDlg 대화 상자

class ErrListDlg : public CDialogEx
{
	DECLARE_DYNAMIC(ErrListDlg)

public:
	ErrListDlg(CWnd* pParent /*=nullptr*/, std::vector<converterInfo>* errList);   // 표준 생성자입니다.
	virtual ~ErrListDlg();
	virtual BOOL OnInitDialog();

private:
	std::vector<converterInfo>* mErrList;
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG1 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	CListCtrl mListErr;
	afx_msg void OnBnClickedRefresh();
	afx_msg void OnBnClickedClose();
};
