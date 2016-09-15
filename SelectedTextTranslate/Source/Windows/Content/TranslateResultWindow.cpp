#include "Windows\Content\TranslateResultWindow.h"

TranslateResultWindow::TranslateResultWindow(Renderer* renderer, AppModel* appModel, HWND parentWindow, HINSTANCE hInstance, DWORD x, DWORD y)
: ContentWindow(renderer, appModel, parentWindow, hInstance, x, y, 0, 0)
{
}

void TranslateResultWindow::Initialize()
{
    ContentWindow::Initialize();

    fontUnderscored = renderer->CreateCustomFont(hWindow, FontSizes::Small, false, true);
}

void TranslateResultWindow::ExpandDictionary(int index)
{
    appModel->ToggleTranslateResultDictionary(index);
}

POINT TranslateResultWindow::RenderDC()
{
    POINT bottomRight = ContentWindow::RenderDC();

    TranslateResult translateResult = appModel->GetCurrentTranslateResult();

    int curY = lineHeight / 4;
    int dY = roundToInt((renderer->kY - 1) * 10);

    for (size_t i = 0; i < translateResult.TranslateCategories.size(); ++i)
    {
        TranslateResultDictionary category = translateResult.TranslateCategories[i];

        // Draw category header
        POINT baseFormBottomRight = renderer->PrintText(inMemoryDC, category.BaseForm, fontNormal, Colors::Black, paddingX, curY, &bottomRight);

        if (_tcslen(category.PartOfSpeech) != 0)
        {
            wstring text = L" - " + wstring(category.PartOfSpeech);
            renderer->PrintText(inMemoryDC, text.c_str(), fontItalic, Colors::Gray, baseFormBottomRight.x + 2, curY, &bottomRight);
        }

        vector<TranslateResultDictionaryEntry> showedEntries(0);
        if (category.IsExtendedList) 
        {
            showedEntries = category.Entries;
        }
        else 
        {
            for (size_t i = 0; i < category.Entries.size(); ++i)
            {
                if (category.Entries[i].Score < 0.003 && i >= 7)
                {
                    continue;
                }

                showedEntries.push_back(category.Entries[i]);
            }
        }

        // Draw words
        curY += lineHeight;
        for (size_t j = 0; j < showedEntries.size(); ++j)
        {
            TranslateResultDictionaryEntry entry = category.Entries[j];
            POINT wordBottomRight = renderer->PrintText(inMemoryDC, entry.Word, fontNormal, Colors::Black, paddingX * 3, curY, &bottomRight);

            // Draw reverse translation
            if (entry.ReverseTranslation.size() != 0)
            {
                wordBottomRight = renderer->PrintText(inMemoryDC, L" - ", fontNormal, Colors::Gray, wordBottomRight.x + 2, curY, &bottomRight);
                for (size_t k = 0; k < entry.ReverseTranslation.size(); ++k)
                {
                    wstring text = wstring(entry.ReverseTranslation[k]);
                    if (k != entry.ReverseTranslation.size() - 1)
                    {
                        text += L", ";
                    }
                    wordBottomRight = renderer->PrintText(inMemoryDC, text.c_str(), fontItalic, Colors::Gray, wordBottomRight.x, curY, &bottomRight);
                }
            }

            int k = entry.Score >= 0.05 ? 0 : (entry.Score >= 0.0025 ? 1 : 2);
            int rateUnit = renderer->AdjustToXResolution(8);

            RECT rect;
            rect.right = paddingX + rateUnit * 3;
            rect.top = curY + lineHeight / 3 - dY*2;
            rect.bottom = rect.top + lineHeight / 3;
            rect.left = paddingX + k * rateUnit;
            renderer->DrawRect(inMemoryDC, rect, grayBrush, &bottomRight);
            curY += lineHeight;
        }

        curY = TranslateResultWindow::CreateExpandButton(category, i, showedEntries.size(), curY, &bottomRight);
        curY += lineHeight / 2;
    }

    bottomRight.y += lineHeight * 3;
    bottomRight.x += paddingX * 3;

    RECT rect;
    rect.left = 0;
    rect.right = bottomRight.x;
    rect.top = 0;
    rect.bottom = 1;
    renderer->DrawRect(inMemoryDC, rect, grayBrush, &POINT());

    return bottomRight;
}

int TranslateResultWindow::CreateExpandButton(
    TranslateResultDictionary category,
    int categoryIndex,
    int showedCount,
    int curY,
    POINT* bottomRight)
{
    DWORD hiddenCount = category.Entries.size() - showedCount;

    if (category.IsExtendedList || hiddenCount > 0) {
        wstring text = L"";

        if (!category.IsExtendedList) {
            if (hiddenCount == 1) {
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
            renderer,
            hWindow,
            hInstance,
            paddingX * 3,
            curY,
            fontUnderscored,
            Colors::Gray,
            Colors::Black,
            text,
            bind(&TranslateResultWindow::ExpandDictionary, this, categoryIndex));

        AddChildWindow(expandButton);

        bottomRight->y += lineHeight / 2;
        curY += lineHeight / 2;
    }

    return curY;
}

TranslateResultWindow::~TranslateResultWindow()
{
    DeleteObject(this->fontUnderscored);
}