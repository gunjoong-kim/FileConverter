#pragma once

#define NEWLINE "\n"

#include "AConsumer.h"

class CBinToTxtConverter : public AConsumer
{
public:
	CBinToTxtConverter(
		CSafeQueue<CString>* jobQueue,
		CString& inputDir,
		CString& outputDir,
		CString& saveDir,
		HWND hwnd,
		UINT handle
	) : AConsumer(jobQueue, inputDir, outputDir, saveDir, hwnd, handle) {}

	virtual converterInfo Do(const CString& inputFullPath);

private:
	converterInfo Convert(const CString& inputFullPath);
};