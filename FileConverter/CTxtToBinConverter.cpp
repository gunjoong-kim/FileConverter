#include "pch.h"
#include "CTxtToBinConverter.h"

converterInfo CTxtToBinConverter::Convert(const CString& inputFullPath)
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

converterInfo CTxtToBinConverter::Do(const CString& inputFullPath)
{
	return Convert(inputFullPath);
}
