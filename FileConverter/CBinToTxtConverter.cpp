#include "pch.h"
#include "CBinToTxtConverter.h"

BOOL CheckHeader(CString& header, CString& outFileName)
{
	//���� �� �ٵ��� ��
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

BOOL CBinToTxtConverter::convert(CString& outputFilePath, CString& inputFilePath)
{
	CFile inputFile;
	if (!inputFile.Open(inputFilePath, CFile::modeRead | CFile::typeBinary))
	{
		AfxMessageBox(CString("Failed to open the bin file. : ") + inputFilePath);
		return FALSE;
	}

	CString header;
	while (true)
	{
		TCHAR currentChar;
		if (inputFile.Read(&currentChar, sizeof(currentChar)) != sizeof(currentChar))
		{
			AfxMessageBox(CString("No Header : ") + inputFilePath);
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
		AfxMessageBox(CString("Invalid Header : ") + inputFilePath);
		return FALSE;
	}

	// �ؽ�Ʈ ���� ����
	CStdioFile outputFile;
	if (!outputFile.Open(outputFilePath + fileName + _T(".atxt"), CFile::modeCreate | CFile::modeWrite | CFile::typeText))
	{
		AfxMessageBox(_T("Failed to open output text file"));
		inputFile.Close();
		return FALSE;
	}

	// ���� ũ�� Ȯ��
	inputFile.SeekToBegin();
	UINT fileSize = (UINT)inputFile.GetLength() * 2;

	// ���̳ʸ� ���Ͽ��� ������ �о����
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