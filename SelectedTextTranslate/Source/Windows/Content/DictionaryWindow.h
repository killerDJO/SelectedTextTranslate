#pragma once
#include "Windows\Content\Base\ContentWindow.h"
#include "Entities\LogRecord.h"
#include "Windows\Buttons\HoverIconButtonWindow.h"

class DictionaryWindow : public ContentWindow
{
protected:
	POINT RenderDC() override;

private:
	void ShowFullTranslation(int dictionaryIndex);

public:
	DictionaryWindow(AppModel* appModel, HWND parentWindow, HINSTANCE hInstance, DWORD x, DWORD y, DWORD width, DWORD height);
	~DictionaryWindow();
};

