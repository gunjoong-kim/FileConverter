#include "pch.h"
#include "CBiConverter.h"

converterInfo CBiConverter::ConvertBinToTxt(const CString& inputFullPath)
{
	converterInfo ret;
	ret.status = FALSE;
	ret.fullPath = inputFullPath;
	ret.startTime = CTime::GetCurrentTime();

	// 올바르지 않은 인코딩인 경우 찾기
	CStdioFile inputFile;
	if (!inputFile.Open(inputFullPath, CFile::modeRead | CFile::typeBinary))
	{
		ret.errMsg = OPEN_FAIL_ERR_MSG;
		return ret;
	}

	if (inputFile.GetLength() < 18)
	{
		ret.errMsg = FILE_SIZE_UNDER_18_ERR_MSG;
		return ret;
	}

	char BOM[2];
	inputFile.Read(&BOM, 2);
	if (BOM[0] != (char)0xFF || BOM[1] != (char)0xFE)
		inputFile.SeekToBegin();

	CString header;
	CString fileName;
	GetNextLine(inputFile, header);
	if (CheckHeader(header, fileName) == FALSE)
	{
		ret.errMsg = INVALID_HEADER_ERR_MSG;
		return ret;
	}
	if (fileName.FindOneOf(_T("\\/:*?\"<>|")) != -1)
	{
		ret.errMsg = INVALID_OUTPUT_NAME_ERR_MSG;
		return ret;
	}
	CString outputDir = mOutputDir;
	CString outputFullPath = outputDir + fileName + _T(".atxt");
	outputFullPath = GetFullPathWithNumbering(outputDir, outputFullPath);

	CStdioFile outputFile;
	if (!outputFile.Open(outputFullPath, CFile::modeCreate | CFile::modeWrite | CFile::typeBinary))
	{
		inputFile.Close();
		ret.errMsg = CREATE_FAIL_ERR_MSG;
		return ret;
	}

	CString content;
	inputFile.SeekToBegin();
	UINT fileSize = (UINT)inputFile.GetLength();
	wchar_t* buffer = new wchar_t[fileSize / sizeof(wchar_t)];
	inputFile.Read(buffer, fileSize);
	outputFile.Write(buffer, fileSize);

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
	ret.startTime = CTime::GetCurrentTime();
	ret.status = FALSE;

	CStdioFile textFile;
	if (!textFile.Open(inputFullPath, CFile::modeRead | CFile::typeText))
	{
		ret.errMsg = OPEN_FAIL_ERR_MSG;
		return ret;
	}

	if (textFile.GetLength() < 18)
	{
		ret.errMsg = FILE_SIZE_UNDER_18_ERR_MSG;
		return ret;
	}

	CString header;
	CString fileName;
	BOOL isBom = FALSE;
	char BOM[2];
	textFile.Read(&BOM, 2);
	if (BOM[0] == (char)0xFF && BOM[1] == (char)0xFE)
		isBom = TRUE;
	else
		textFile.SeekToBegin();

	GetNextLine(textFile, header);
	if (CheckHeader(header, fileName) == FALSE)
	{
		ret.errMsg = INVALID_HEADER_ERR_MSG;
		return ret;
	}
	if (fileName.FindOneOf(_T("\\/:*?\"<>|")) != -1)
	{
		ret.errMsg = INVALID_OUTPUT_NAME_ERR_MSG;
		return ret;
	}

	CString outputDir = mOutputDir;
	CString outputFullPath = outputDir + fileName + _T(".abin");
	outputFullPath = GetFullPathWithNumbering(outputDir, outputFullPath);
	CStdioFile binaryFile;
	if (!binaryFile.Open(outputFullPath, CFile::modeCreate | CFile::modeWrite | CFile::typeBinary))
	{
		textFile.Close();
		ret.errMsg = CREATE_FAIL_ERR_MSG;
		return ret;
	}

	CString content;
	textFile.SeekToBegin();
	UINT fileSize = (UINT)textFile.GetLength();
	wchar_t* buffer = new wchar_t[fileSize / sizeof(wchar_t)];
	textFile.Read(buffer, fileSize);
	binaryFile.Write(buffer, fileSize);
	delete[] buffer;
	textFile.Close();
	binaryFile.Close();

	ret.endTime = CTime::GetCurrentTime();
	ret.status = TRUE;
	return ret;
}

converterInfo CBiConverter::Do(const CString& inputFullPath)
{
	converterInfo ret;
	CString fileName = GetFileName(inputFullPath);
	ret.fullPath = inputFullPath;
	ret.status = FALSE;
	ret.startTime = CTime::GetCurrentTime();
	if (fileName.Find(_T("[TargetFileName] ")) != 0)
	{
		ret.errMsg = INVALID_FILENAME_ERR_MSG;
		return ret;
	}
	CString extension = GetExtension(inputFullPath);
	if (extension == _T("abin"))
		return ConvertBinToTxt(inputFullPath);
	else if (extension == _T("atxt"))
		return ConvertTxtToBin(inputFullPath);
	ret.errMsg = INVALID_EXTENSION_ERR_MSG;
	return ret;
}
