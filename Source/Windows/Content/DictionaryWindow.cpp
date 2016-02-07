#include "PrecompiledHeaders\stdafx.h"
#include "Windows\Content\DictionaryWindow.h"

DictionaryWindow::DictionaryWindow(HWND parentWindow, HINSTANCE hInstance, DWORD x, DWORD y, DWORD width, DWORD height)
	: ContentWindow(parentWindow, hInstance, x, y, width, height)
{
	SetWindowLongPtr(this->hWindow, GWL_WNDPROC, (LONG_PTR)DictionaryWindow::WndProc);
}

LRESULT CALLBACK DictionaryWindow::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	DictionaryWindow* instance = (DictionaryWindow*)GetWindowLongPtr(hWnd, GWLP_USERDATA);

	switch (message)
	{
	case WM_COMMAND:
		if (LOWORD(wParam) == BN_CLICKED)
		{
			//instance->PlayText();
		}
		break;
	}

	return ContentWindow::WndProc(hWnd, message, wParam, lParam);
}

POINT DictionaryWindow::RenderResult(vector<LogRecord> records)
{
	this->records = records;
	return ContentWindow::RenderResult();
}

POINT DictionaryWindow::RenderDC()
{
	ContentWindow::RenderDC();

	POINT bottomRight = { 0, 0 };

	int curY = PADDING_Y / 2;

	for (size_t i = 0; i < this->records.size(); ++i) 
	{
		LogRecord record = records[i];
		POINT lineBottomRight = Utilities::PrintText(this->inMemoryHDC, Utilities::GetWideChar(record.Word), fontNormal, COLOR_BLACK, PADDING_X * 2, curY, &bottomRight);
		Utilities::PrintText(inMemoryHDC, Utilities::GetWideChar(" (" + to_string(record.Count) + ")"), fontNormal, COLOR_GRAY, lineBottomRight.x, curY, &bottomRight);
		
		curY += LINE_HEIGHT;
	}
	
	return bottomRight;
}

DictionaryWindow::~DictionaryWindow()
{
}
