#pragma once
#include "ContentWindow.h"
#include "TranslateResult.h"
#include "Utilities.h"

class TranslateResultWindow : public ContentWindow
{
public:
	TranslateResultWindow(HWND parentWindow, HINSTANCE hInstance, DWORD x, DWORD y);
	~TranslateResultWindow();
	virtual void Draw();
};