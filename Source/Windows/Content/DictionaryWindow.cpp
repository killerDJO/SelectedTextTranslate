#include "PrecompiledHeaders\stdafx.h"
#include "Windows\Content\DictionaryWindow.h"
#include "Windows\Buttons\TranslateButtonWindow.h"
#include "Windows\Buttons\RemoveButtonWindow.h"

#define GWL_TYPE_TRANSLATE 42002
#define GWL_TYPE_REMOVE 42003

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
		if (HIWORD(wParam) == STN_DBLCLK || HIWORD(wParam) == STN_CLICKED)
		{
			int dictionaryIndex = GetWindowLong((HWND)lParam, GWL_ID);
			int type = GetWindowLong((HWND)lParam, GWL_USERDATA);

			if (type == GWL_TYPE_REMOVE) 
			{
				//MessageBox(instance->parentWindow, L"Are you sure you want to delete the item?", L"Confirm", MB_OKCANCEL | MB_DEFBUTTON1 | MB_TASKMODAL);
				//SendMessage(instance->parentWindow, WM_NOTIFY, WM_TRANLSATE_LOG_RECORD, dictionaryIndex);
			}
			else if (type == GWL_TYPE_TRANSLATE)
			{
				SendMessage(instance->parentWindow, WM_NOTIFY, WM_TRANLSATE_LOG_RECORD, dictionaryIndex);
			}
			
			return TRUE;
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
		POINT lineBottomRight = Utilities::PrintText(this->inMemoryHDC, Utilities::GetWideChar(record.Word), fontNormal, COLOR_BLACK, PADDING_X * 2 + 4, curY, &bottomRight);
		Utilities::PrintText(inMemoryHDC, Utilities::GetWideChar(" (" + to_string(record.Count) + ")"), fontNormal, COLOR_GRAY, lineBottomRight.x + 1, curY, &bottomRight);
		
		TranslateButtonWindow* translateButton = new TranslateButtonWindow(
			this->hWindow,
			this->hInstance,
			PADDING_X,
			curY + 2,
			13,
			13);
		SetWindowLong(translateButton->GetHandle(), GWL_ID, i);
		SetWindowLong(translateButton->GetHandle(), GWL_USERDATA, GWL_TYPE_TRANSLATE);

		curY += LINE_HEIGHT;
	}
	
	return bottomRight;
}

DictionaryWindow::~DictionaryWindow()
{
}
