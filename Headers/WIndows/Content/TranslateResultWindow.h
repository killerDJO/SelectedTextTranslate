#pragma once
#include "Windows\Content\Base\ContentWindow.h"
#include "Entities\TranslateResult.h"
#include "Helpers\Utilities.h"

class TranslateResultWindow : public ContentWindow
{
private:
	virtual POINT RenderDC();

public:
	TranslateResultWindow(HWND parentWindow, HINSTANCE hInstance, DWORD x, DWORD y, DWORD width, DWORD height);
	~TranslateResultWindow();	
};