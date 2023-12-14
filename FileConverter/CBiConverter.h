#pragma once

#define NEWLINE "\r\n"

#include "AConsumer.h"

class CBiConverter : public AConsumer
{
public:
	CBiConverter(
		UINT threadIdx,
		CSafeQueue<CString>* jobQueue,
		CString& inputDir,
		CString& outputDir,
		CString& saveDir,
		HWND hwnd,
		UINT handle
	) : AConsumer(threadIdx, jobQueue, inputDir, outputDir, saveDir, hwnd, handle) {}
	virtual converterInfo Do(const CString& inputFullPath);

private:
	converterInfo ConvertBinToTxt(const CString& inputFullPath);
	converterInfo ConvertTxtToBin(const CString& inputFullPath);
};
