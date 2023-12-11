#pragma once

#define NEWLINE "\n"

#include "AConsumer.h"

class CBiConverter : public AConsumer
{
public:
	CBiConverter(
		CSafeQueue<CString>* jobQueue,
		CString& inputDir,
		CString& outputDir,
		CString& saveDir,
		HWND hwnd,
		UINT handle
	) : AConsumer(jobQueue, inputDir, outputDir, saveDir, hwnd, handle) {}
	virtual converterInfo Do(const CString& inputFullPath);

private:
	converterInfo ConvertBinToTxt(const CString& inputFullPath);
	converterInfo ConvertTxtToBin(const CString& inputFullPath);
};
