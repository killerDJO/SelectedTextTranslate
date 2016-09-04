#include "Windows\Content\TranslateResultWindow.h"

TranslateResultWindow::TranslateResultWindow(AppModel* appModel, HWND parentWindow, HINSTANCE hInstance, DWORD x, DWORD y, DWORD width, DWORD height)
: ContentWindow(appModel, parentWindow, hInstance, x, y, width, height)
{
}

void TranslateResultWindow::InitializeFonts()
{
	ContentWindow::InitializeFonts();

	HDC hdc = GetDC(hWindow);
	long lfHeightSmall = -MulDiv(int(FONT_HEIGHT * 3 / 7.0), GetDeviceCaps(hdc, LOGPIXELSY), 72);
	this->fontUnderscored = CreateFont(lfHeightSmall, 0, 0, 0, 0, 0, TRUE, 0, 0, 0, 0, 0, 0, TEXT("Arial"));
}

void TranslateResultWindow::ExpandDictionary(int index)
{
	this->appModel->ToggleTranslateResultDictionary(index);
}

POINT TranslateResultWindow::RenderDC()
{	
	ContentWindow::RenderDC();

	TranslateResult translateResult = this->appModel->GetCurrentTranslateResult();

	POINT bottomRight = { 0, 0 };
	int curY = LINE_HEIGHT / 4;
	int dY = int((kY - 1) * 10);

	for (size_t i = 0; i < translateResult.TranslateCategories.size(); ++i)
	{
		TranslateResultDictionary category = translateResult.TranslateCategories[i];

		// Draw category header
		POINT baseFormBottomRight = PrintText(inMemoryHDC, category.BaseForm, fontNormal, COLOR_BLACK, PADDING_X, curY, &bottomRight);

		if (_tcslen(category.PartOfSpeech) != 0)
		{
			wstring text = L" - " + wstring(category.PartOfSpeech);
			PrintText(inMemoryHDC, const_cast<wchar_t*>(text.c_str()), fontItalic, COLOR_GRAY, baseFormBottomRight.x + 2, curY, &bottomRight);
		}
		
		vector<TranslateResultDictionaryEntry> showedEntries(0);
		if (category.IsExtendedList) {
			showedEntries = category.Entries;
		}
		else {
			for (size_t i = 0; i < category.Entries.size(); ++i){
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
			POINT wordBottomRight = PrintText(inMemoryHDC, entry.Word, fontNormal, COLOR_BLACK, PADDING_X * 3, curY, &bottomRight);

			// Draw reverse translation
			if (entry.ReverseTranslation.size() != 0)
			{
				wordBottomRight = PrintText(inMemoryHDC, L" - ", fontNormal, COLOR_GRAY, wordBottomRight.x + 2, curY, &bottomRight);
				for (size_t k = 0; k < entry.ReverseTranslation.size(); ++k)
				{
					wstring text = wstring(entry.ReverseTranslation[k]);
					if (k != entry.ReverseTranslation.size() - 1)
					{
						text += L", ";
					}
					wordBottomRight = PrintText(inMemoryHDC, const_cast<wchar_t*>(text.c_str()), fontItalic, COLOR_GRAY, wordBottomRight.x, curY, &bottomRight);
				}
			}			

			int k = entry.Score >= 0.05 ? 0 : (entry.Score >= 0.0025 ? 1 : 2);
			int rateUnit = AdjustToResolution(8, kX);

			RECT rect;
			rect.right = PADDING_X + rateUnit * 3;
			rect.top = curY + LINE_HEIGHT / 3 - dY*2;
			rect.bottom = rect.top + LINE_HEIGHT / 3;
			rect.left = PADDING_X + k * rateUnit;
			DrawRect(inMemoryHDC, rect, this->grayBrush, &bottomRight);
			curY += LINE_HEIGHT;
		}

		DWORD hiddenCount = category.Entries.size() - showedEntries.size();
		if (category.IsExtendedList || hiddenCount > 0){
				wstring text = L"";
				if (!category.IsExtendedList){
					if (hiddenCount == 1){
						text = L"show " + to_wstring(hiddenCount) + L" more result";
					}
					else {
						text = L"show " + to_wstring(hiddenCount) + L" more results";
					}
				}
				else {
					text = L"show less results";
				}

			HoverTextButtonWindow* expandButton = new HoverTextButtonWindow(
				this->hWindow,
				this->hInstance,
				PADDING_X * 3,
				curY,
				this->fontUnderscored,
				RGB(119, 119, 119),
				RGB(0, 0, 0),
				text,
				bind(&TranslateResultWindow::ExpandDictionary, this, i));
			
			expandButton->Initialize();
			childWindows.push_back(expandButton);

			bottomRight.y += LINE_HEIGHT / 2;
			curY += LINE_HEIGHT / 2;
		}

		curY += LINE_HEIGHT/2;
	}

	bottomRight.y += LINE_HEIGHT * 3;
	bottomRight.x += PADDING_X * 3;

	return bottomRight;
}

TranslateResultWindow::~TranslateResultWindow()
{
	DeleteObject(this->fontUnderscored);
}                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                           