#include "PrecompiledHeaders\stdafx.h"
#include "Windows\Content\TranslateResultWindow.h"

TranslateResultWindow::TranslateResultWindow(HWND parentWindow, HINSTANCE hInstance, DWORD x, DWORD y) 
: ContentWindow(parentWindow, hInstance, x, y, 2000, 3000){ }

POINT TranslateResultWindow::RenderDC()
{	
	ContentWindow::RenderDC();

	POINT bottomRight = { 0, 0 };
	int curY = 0;

	for (size_t i = 0; i < translateResult.TranslateCategories.size(); ++i)
	{
		TranslateResultDictionary category = translateResult.TranslateCategories[i];

		// Draw category header
		POINT baseFormBottomRight = Utilities::PrintText(inMemoryHDC, category.BaseForm, fontNormal, COLOR_BLACK, PADDING, curY, &bottomRight);

		if (_tcslen(category.PartOfSpeech) != 0)
		{
			wstring text = L" - " + wstring(category.PartOfSpeech);
			Utilities::PrintText(inMemoryHDC, const_cast<wchar_t*>(text.c_str()), fontItalic, COLOR_GRAY, baseFormBottomRight.x + 2, curY, &bottomRight);
		}

		// Draw words
		curY += LINE_HEIGHT;
		for (size_t j = 0; j < category.Entries.size(); ++j)
		{
			TranslateResultDictionaryEntry entry = category.Entries[j];
			POINT wordBottomRight = Utilities::PrintText(inMemoryHDC, entry.Word, fontNormal, COLOR_BLACK, PADDING * 3, curY, &bottomRight);

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
			int rateUnit = 8;

			RECT rect;
			rect.right = PADDING + rateUnit * 3;
			rect.top = curY + LINE_HEIGHT / 3;
			rect.bottom = rect.top + LINE_HEIGHT / 3;
			rect.left = PADDING + k * rateUnit;
			Utilities::DrawRect(inMemoryHDC, rect, this->grayBrush, &bottomRight);
			curY += LINE_HEIGHT;
		}

		curY += LINE_HEIGHT/2;
	}

	bottomRight.y += LINE_HEIGHT * 3;
	bottomRight.x += PADDING * 3;

	return bottomRight;
}

TranslateResultWindow::~TranslateResultWindow()
{	
}                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                           