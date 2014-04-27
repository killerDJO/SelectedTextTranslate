#include "PrecompiledHeaders\stdafx.h"
#include "Windows\Content\TranslateResultWindow.h"

TranslateResultWindow::TranslateResultWindow(HWND parentWindow, HINSTANCE hInstance, DWORD x, DWORD y, DWORD width, DWORD height) 
: ContentWindow(parentWindow, hInstance, x, y, width, height){ }

POINT TranslateResultWindow::RenderDC()
{	
	ContentWindow::RenderDC();

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
		
		vector<TranslateResultDictionaryEntry> entries(0);
		if (category.IsExtendedList) {
			entries = category.Entries;
		}
		else {
			for (int i = 0; i < category.Entries.size(); ++i){
				if (category.Entries[i].Score < 0.003){
					continue;
				}
				entries.push_back(category.Entries[i]);
			}
		}
		
		// Draw words
		curY += LINE_HEIGHT;
		for (size_t j = 0; j < entries.size(); ++j)
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

		if (category.IsExtendedList){
			Utilities::PrintText(inMemoryHDC, L"Hide results", fontNormal, COLOR_GRAY, PADDING_X * 3, curY, &bottomRight);
		}
		else{
			Utilities::PrintText(inMemoryHDC, L"Expand results", fontNormal, COLOR_GRAY, PADDING_X * 3, curY, &bottomRight);
		}
		curY += LINE_HEIGHT;

		curY += LINE_HEIGHT/2;
	}

	bottomRight.y += LINE_HEIGHT * 3;
	bottomRight.x += PADDING_X * 3;

	return bottomRight;
}

TranslateResultWindow::~TranslateResultWindow()
{	
}                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                           