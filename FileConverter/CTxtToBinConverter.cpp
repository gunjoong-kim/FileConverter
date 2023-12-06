#include "pch.h"
#include "CTxtToBinConverter.h"

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

BOOL CTxtToBinConverter::convert(CString& outputFilePath, CString& inputFilePath)
{
	CStdioFile textFile;
	if (!textFile.Open(inputFilePath, CFile::modeRead | CFile::typeText))
	{
		AfxMessageBox(CString("Failed to open the text file. : ") + inputFilePath);
		return FALSE;
	}

	CString header;
	if (!textFile.ReadString(header))
	{
		AfxMessageBox(CString("No Header : ") + inputFilePath);
		return FALSE;
	}

	CString fileName;
	if (CheckHeader(header, fileName) == FALSE)
	{
		AfxMessageBox(CString("Invalid Header : ") + inputFilePath);
		return FALSE;
	}

	CFile binaryFile;
	if (!binaryFile.Open(outputFilePath + fileName + _T(".abin"), CFile::modeCreate | CFile::modeWrite | CFile::typeBinary))
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