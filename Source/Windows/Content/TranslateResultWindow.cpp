#include "PrecompiledHeaders\stdafx.h"
#include "Windows\Content\TranslateResultWindow.h"

TranslateResultWindow::TranslateResultWindow(MainWindow* mainWindow, DWORD x, DWORD y, DWORD width, DWORD height)
: ContentWindow(mainWindow->GetHandle(), mainWindow->GetInstance(), x, y, width, height)
{ 
	this->mainWindow = mainWindow;
	SetWindowLongPtr(this->hWindow, GWL_WNDPROC, (LONG_PTR)TranslateResultWindow::WndProc);
}

LRESULT CALLBACK TranslateResultWindow::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	TranslateResultWindow* instance = (TranslateResultWindow*)GetWindowLongPtr(hWnd, GWLP_USERDATA);
	HBRUSH hbrBkgnd = CreateSolidBrush(RGB(255, 255, 255));
	switch (message)
	{
	case WM_COMMAND:
		if (LOWORD(wParam) == STN_DBLCLK || LOWORD(wParam) == STN_CLICKED)
		{
			int dictionaryIndex = GetWindowLong((HWND)lParam, GWL_ID);
			instance->ExpandDictionary(dictionaryIndex);
			return TRUE;
		}
		case WM_CTLCOLORSTATIC:
		{
			HDC hdcStatic = (HDC)wParam;
			long hovered = GetWindowLong((HWND)lParam, GWL_USERDATA);
			if (hovered){
				SetTextColor(hdcStatic, RGB(0, 0, 0));
			}
			else {
				SetTextColor(hdcStatic, RGB(119, 119, 119));
			}

			SetBkColor(hdcStatic, RGB(255, 255, 255));
			return (INT_PTR)hbrBkgnd;
		}
		break;
	}
	return ContentWindow::WndProc(hWnd, message, wParam, lParam);
}

void TranslateResultWindow::ExpandDictionary(int index)
{
	translateResult.TranslateCategories[index].IsExtendedList ^= true;
	
	SCROLLINFO si;
	si.cbSize = sizeof (si);
	si.fMask = SIF_POS;
	GetScrollInfo(this->parentWindow, SB_VERT, &si);

	mainWindow->Render(translateResult, si.nPos);
}

POINT TranslateResultWindow::RenderDC()
{	
	ContentWindow::RenderDC();

	this->DestroyButtons();

	POINT bottomRight = { 0, 0 };
	int curY = LINE_HEIGHT / 4;

	for (size_t i = 0; i < translateResult.TranslateCategories.size(); ++i)
	{
		TranslateResultDictionary category = translateResult.TranslateCategories[i];

		// Draw category header
		POINT baseFormBottomRight = Utilities::PrintText(inMemoryHDC, category.BaseForm, fontNormal, COLOR_BLACK, PADDING_X, curY, &bottomRight);

		if (_tcslen(category.PartOfSpeech) != 0)
		{
			wstring text = L" - " + wstring(category.PartOfSpeech);
			Utilities::PrintText(inMemoryHDC, const_cast<wchar_t*>(text.c_str()), fontItalic, COLOR_GRAY, baseFormBottomRight.x + 2, curY, &bottomRight);
		}
		
		vector<TranslateResultDictionaryEntry> showedEntries(0);
		if (category.IsExtendedList) {
			showedEntries = category.Entries;
		}
		else {
			for (int i = 0; i < category.Entries.size(); ++i){
				if (category.Entries[i].Score < 0.003 && i >= 7){
					continue;
				}
				showedEntries.push_back(category.Entries[i]);
			}
		}
		
		// Draw words
		curY += LINE_HEIGHT;
		for (size_t j = 0; j < showedEntries.size(); ++j)
		{
			TranslateResultDictionaryEntry entry = category.Entries[j];
			POINT wordBottomRight = Utilities::PrintText(inMemoryHDC, entry.Word, fontNormal, COLOR_BLACK, PADDING_X * 3, curY, &bottomRight);

			// Draw reverse translation
			if (entry.ReverseTranslation.size() != 0)
			{
				wordBottomRight = Utilities::PrintText(inMemoryHDC, L" - ", fontNormal, COLOR_GRAY, wordBottomRight.x + 2, curY, &bottomRight);
				for (size_t k = 0; k < entry.ReverseTranslation.size(); ++k)
				{
					wstring text = wstring(entry.ReverseTranslation[k]);
					if (k != entry.ReverseTranslation.size() - 1)
					{
						text += L", ";
					}
					wordBottomRight = Utilities::PrintText(inMemoryHDC, const_cast<wchar_t*>(text.c_str()), fontItalic, COLOR_GRAY, wordBottomRight.x, curY, &bottomRight);
				}
			}			

			int k = entry.Score >= 0.05 ? 0 : (entry.Score >= 0.003 ? 1 : 2);
			int rateUnit = AdjustToResolution(8, kX);

			RECT rect;
			rect.right = PADDING_X + rateUnit * 3;
			rect.top = curY + LINE_HEIGHT / 3;
			rect.bottom = rect.top + LINE_HEIGHT / 3;
			rect.left = PADDING_X + k * rateUnit;
			Utilities::DrawRect(inMemoryHDC, rect, this->grayBrush, &bottomRight);
			curY += LINE_HEIGHT;
		}

		DWORD hiddentCount = category.Entries.size() - showedEntries.size();
		if (category.IsExtendedList || hiddentCount > 0){
			ExpandButtonWindow* expandButton = new ExpandButtonWindow(
				this->hWindow,
				this->hInstance,
				PADDING_X * 3,
				curY,
				2000,
				LINE_HEIGHT,
				hiddentCount,
				category.IsExtendedList);
			
			SetWindowLong(expandButton->GetHandle(), GWL_ID, i);
			expandButtons.push_back(expandButton);

			bottomRight.y += LINE_HEIGHT / 2;
			curY += LINE_HEIGHT / 2;
		}

		curY += LINE_HEIGHT/2;
	}

	bottomRight.y += LINE_HEIGHT * 3;
	bottomRight.x += PADDING_X * 3;

	return bottomRight;
}

void TranslateResultWindow::DestroyButtons()
{
	for (int i = 0; i < expandButtons.size(); ++i){
		expandButtons[i]->Destroy();
	}
	expandButtons.clear();
	expandButtons.resize(0);
}
TranslateResultWindow::~TranslateResultWindow()
{	
}                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                           