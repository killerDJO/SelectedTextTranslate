#include "PrecompiledHeaders\stdafx.h"
#include "Windows\Content\DictionaryWindow.h"

#define GWL_TYPE_TRANSLATE 42002
#define GWL_TYPE_REMOVE 42003

DictionaryWindow::DictionaryWindow(HWND parentWindow, HINSTANCE hInstance, DWORD x, DWORD y, DWORD width, DWORD height)
	: ContentWindow(parentWindow, hInstance, x, y, width, height)
{
}

void DictionaryWindow::ShowFullTranslation(int dictionaryIndex)
{
	SendMessage(this->parentWindow, WM_NOTIFY, WM_TRANLSATE_LOG_RECORD, dictionaryIndex);
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

	size_t countToShow = min(200, records.size());

	wstring title = L"Showing " + to_wstring(countToShow) + L" out of " + to_wstring(records.size());
	POINT lineBottomRight = Utilities::PrintText(this->inMemoryHDC, title.c_str(), fontItalic, COLOR_GRAY, PADDING_X, curY, &bottomRight);
	
	curY = lineBottomRight.y + PADDING_Y / 2;
	
	for (size_t i = 0; i < countToShow; ++i)
	{
		LogRecord record = records[i];
		POINT lineBottomRight = Utilities::PrintText(this->inMemoryHDC, record.Word.c_str(), fontNormal, COLOR_BLACK, PADDING_X * 2 + 4, curY, &bottomRight);
		Utilities::PrintText(inMemoryHDC, wstring(L" (" + to_wstring(record.Count) + L")").c_str(), fontNormal, COLOR_GRAY, lineBottomRight.x + 1, curY, &bottomRight);
		
		HoverIconButtonWindow* translateButton = new HoverIconButtonWindow(
			this->hWindow,
			this->hInstance,
			PADDING_X,
			curY + 2,
			13,
			13,
			IDB_TRANSLATE_INACTIVE,
			IDB_TRANSLATE,
			bind(&DictionaryWindow::ShowFullTranslation, this, i));

		translateButton->Initialize();
		childWindows.push_back(translateButton);

		curY += LINE_HEIGHT;
	}
	
	return bottomRight;
}

DictionaryWindow::~DictionaryWindow()
{
}
