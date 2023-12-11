#pragma once
class CViewer
{
public:
	static UINT ViewTxt(CString& filePath, CEdit& contentBox);
	static UINT ViewBin(CString& filePath, CEdit& contentBox);
};

