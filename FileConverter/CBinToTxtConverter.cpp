#include "pch.h"
#include "CBinToTxtConverter.h"

converterInfo CBinToTxtConverter::Convert(const CString& inputFullPath)
{
	converterInfo ret;
	ret.status = FALSE;
	ret.fullPath = inputFullPath;

	CFile inputFile;
	if (!inputFile.Open(inputFullPath, CFile::modeRead | CFile::typeBinary))
	{
		ret.errMsg = OPEN_FAIL_ERR_MSG;
		return ret;
	}

	CString header;
	while (true)
	{
		TCHAR currentChar;
		if (inputFile.Read(&currentChar, sizeof(currentChar)) != sizeof(currentChar))
		{
			ret.errMsg = INVALID_HEADER_ERR_MSG;
			return ret;
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
		ret.errMsg = INVALID_HEADER_ERR_MSG;
		return ret;
	}

	// 텍스트 파일 생성
	CStdioFile outputFile;
	if (!outputFile.Open(mOutputDir + fileName + _T(".atxt"), CFile::modeCreate | CFile::modeWrite | CFile::typeText))
	{
		inputFile.Close();
		ret.errMsg = CREATE_FAIL_ERR_MSG;
		return ret;
	}

	inputFile.SeekToBegin();
	UINT fileSize = (UINT)inputFile.GetLength() * 2;
	CString str;
	TCHAR* buffer = new TCHAR[fileSize / sizeof(TCHAR)];
	inputFile.Read(buffer, fileSize);
	str.SetString(buffer, fileSize / sizeof(TCHAR));
	outputFile.WriteString(str);

	delete[] buffer;
	inputFile.Close();
	outputFile.Close();

	ret.status = TRUE;
	return ret;
}

converterInfo CBinToTxtConverter::Do(const CString& inputFullPath)
{
	return Convert(inputFullPath);
}
