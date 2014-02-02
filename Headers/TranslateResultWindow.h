#pragma once
#include "ContentWindow.h"
#include "TranslateResult.h"
#include "Utilities.h"

class TranslateResultWindow : public ContentWindow
{
private:
	virtual POINT RenderDC();

public:
	TranslateResultWindow(HWND parentWindow, HINSTANCE hInstance, DWORD x, DWORD y);
	~TranslateResultWindow();	
};