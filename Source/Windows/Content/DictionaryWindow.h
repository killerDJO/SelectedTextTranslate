#pragma once
#include "Windows\Content\Base\ContentWindow.h"
#include "Entities\LogRecord.h"

class DictionaryWindow : public ContentWindow
{
	static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
	virtual POINT RenderDC();

	vector<LogRecord> records;

public:
	DictionaryWindow(HWND parentWindow, HINSTANCE hInstance, DWORD x, DWORD y, DWORD width, DWORD height);
	~DictionaryWindow();

	POINT RenderResult(vector<LogRecord> records);
};

