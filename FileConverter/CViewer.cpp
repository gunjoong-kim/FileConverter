#include "pch.h"
#include "CViewer.h"

UINT CViewer::ViewTxt(CString& filePath, CEdit& contentBox)
{
    CFile file;
    contentBox.SetWindowText(_T(""));
    if (file.Open(filePath, CFile::modeRead | CFile::typeBinary))
    {
        ULONGLONG fileSize = file.GetLength();
        wchar_t* fileContent = new wchar_t[fileSize / sizeof(wchar_t) + 1];
        file.Read(fileContent, fileSize);
        fileContent[fileSize / sizeof(wchar_t)] = L'\0';
        file.Close();

        CString content(fileContent);
        OutputDebugString(content);
        delete[] fileContent;
        contentBox.SetWindowText(content);
        return 0;
    }
    else
    {
        contentBox.SetWindowTextW(_T("파일 열기를 실패하였습니다."));
        return 1;
    }
}

UINT CViewer::ViewBin(CString& filePath, CEdit& contentBox)
{
    CFile file;
    contentBox.SetWindowText(_T(""));
    if (file.Open(filePath, CFile::modeRead | CFile::typeBinary))
    {
        UINT fileSize = static_cast<UINT>(file.GetLength());
        CString fileContent;
        const int blockSize = 24;
        BYTE buffer[blockSize];
        int bytesRead = 0;
        CString binary;

        while (bytesRead = file.Read(buffer, blockSize))
        {
            // 읽은 바이트를 CString에 추가
            for (int i = 0; i < bytesRead; i++)
            {
                binary.Format(_T("%02X "), buffer[i]);
                fileContent += binary;
            }
            fileContent += _T("\r\n");
        }
        for (int i = 0; i < bytesRead; i++)
        {
            binary.Format(_T("%02X "), buffer[i]);
            fileContent += binary;
        }
        fileContent += _T("\r\n");
        contentBox.SetWindowText(fileContent);
        file.Close();
        return 0;
    }
    else
    {
        contentBox.SetWindowTextW(_T("파일 열기를 실패하였습니다."));
        return 1;
    }
}