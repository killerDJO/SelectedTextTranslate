#pragma once
#include "Windows\Content\Base\ContentWindow.h"
#include "Entities\LogRecord.h"
#include "Windows\Buttons\HoverIconButtonWindow.h"

class DictionaryWindow : public ContentWindow
{
protected:
	POINT RenderDC() override;

private:
	vector<LogRecord> records;

	void ShowFullTranslation(int dictionaryIndex);

public:
	DictionaryWindow(HWND parentWindow, HINSTANCE hInstance, DWORD x, DWORD y, DWORD width, DWORD height);
	~DictionaryWindow();

	POINT RenderResult(vector<LogRecord> records);
};

