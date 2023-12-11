#include "pch.h"
#include "CBiConverter.h"

converterInfo CBiConverter::ConvertBinToTxt(const CString& inputFullPath)
{
	converterInfo ret;
	ret.status = FALSE;
	ret.fullPath = inputFullPath;
	ret.startTime = CTime::GetCurrentTime();

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

	ret.endTime = CTime::GetCurrentTime();
	ret.status = TRUE;
	return ret;
}

converterInfo CBiConverter::ConvertTxtToBin(const CString& inputFullPath)
{
	converterInfo ret;
	ret.fullPath = inputFullPath;
	ret.status = FALSE;
	CStdioFile textFile;
	if (!textFile.Open(inputFullPath, CFile::modeRead | CFile::typeText))
	{
		ret.errMsg = OPEN_FAIL_ERR_MSG;
		return ret;
	}

	CString header;
	CString fileName;
	if (textFile.ReadString(header) == FALSE || CheckHeader(header, fileName) == FALSE)
	{
		ret.errMsg = INVALID_HEADER_ERR_MSG;
		return ret;
	}

	CFile binaryFile;
	if (!binaryFile.Open(mOutputDir + fileName + _T(".abin"), CFile::modeCreate | CFile::modeWrite | CFile::typeBinary))
	{
		textFile.Close();
		ret.errMsg = CREATE_FAIL_ERR_MSG;
		return ret;
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

	ret.status = TRUE;
	return ret;
}

converterInfo CBiConverter::Do(const CString& inputFullPath)
{
	converterInfo ret;
	if (inputFullPath.Right(5) == _T(".abin"))
		ret = ConvertBinToTxt(inputFullPath);
	else if (inputFullPath.Right(5) == _T(".atxt"))
		ret = ConvertTxtToBin(inputFullPath);
	return ret;
}
