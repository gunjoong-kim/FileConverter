#pragma once
#include "afxdialogex.h"


// CConverter 대화 상자

class CConverter : public CDialogEx
{
	DECLARE_DYNAMIC(CConverter)

public:
	CConverter(CWnd* pParent /*=nullptr*/, const CString& input, const CString& output, const CString& save);
	virtual ~CConverter();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_CONVERT };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()

private:
	CString inputPath;
	CString outputPath;
	CString savePath;
public:
	afx_msg void OnBnClickedOk();
};
