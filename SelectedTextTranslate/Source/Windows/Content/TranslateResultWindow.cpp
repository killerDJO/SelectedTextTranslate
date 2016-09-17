#include "Windows\Content\TranslateResultWindow.h"

TranslateResultWindow::TranslateResultWindow(HINSTANCE hInstance, RenderingContext* renderingContext, ScrollProvider* scrollProvider, WindowDescriptor descriptor, HWND parentWindow, AppModel* appModel)
: ContentWindow(hInstance, renderingContext, scrollProvider, descriptor, parentWindow, appModel)
{
}

void TranslateResultWindow::Initialize()
{
    ContentWindow::Initialize();

    fontUnderscored = renderingContext->CreateCustomFont(hWindow, FontSizes::Small, false, true);
}

void TranslateResultWindow::ExpandDictionary(int index)
{
    appModel->ToggleTranslateResultDictionary(index);
}

SIZE TranslateResultWindow::RenderDC(Renderer* renderer)
{
    ContentWindow::RenderDC(renderer);

    TranslateResult translateResult = appModel->GetCurrentTranslateResult();

    int curY = lineHeight / 4;

    for (size_t i = 0; i < translateResult.TranslateCategories.size(); ++i)
    {
        TranslateResultDictionary category = translateResult.TranslateCategories[i];

        // Draw category header
        POINT baseFormBottomRight = renderer->PrintText(category.BaseForm, fontNormal, Colors::Black, paddingX, curY);

        if (_tcslen(category.PartOfSpeech) != 0)
        {
            wstring text = L" - " + wstring(category.PartOfSpeech);
            renderer->PrintText(text.c_str(), fontItalic, Colors::Gray, baseFormBottomRight.x, curY);
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
            POINT wordBottomRight = renderer->PrintText(entry.Word, fontNormal, Colors::Black, paddingX * 3, curY);

            // Draw reverse translation
            if (entry.ReverseTranslation.size() != 0)
            {
                wordBottomRight = renderer->PrintText(L" - ", fontNormal, Colors::Gray, wordBottomRight.x + 2, curY);
                for (size_t k = 0; k < entry.ReverseTranslation.size(); ++k)
                {
                    wstring text = wstring(entry.ReverseTranslation[k]);
                    if (k != entry.ReverseTranslation.size() - 1)
                    {
                        text += L", ";
                    }
                    wordBottomRight = renderer->PrintText(text.c_str(), fontItalic, Colors::Gray, wordBottomRight.x, curY);
                }
            }

            int k = entry.Score >= 0.05 ? 0 : (entry.Score >= 0.0025 ? 1 : 2);
            int rateUnit = 8;

            RECT rect;
            rect.right = paddingX + rateUnit * 3;
            rect.top = curY + 5;
            rect.bottom = rect.top + 8;
            rect.left = paddingX + k * rateUnit;
            renderer->DrawRect(rect, grayBrush);
            curY += lineHeight;
        }

        curY = CreateExpandButton(category, i, showedEntries.size(), curY, renderer);
        curY += lineHeight / 2;
    }

    renderer->IncreaseWidth(paddingX * 3);
    renderer->IncreaseHeight(lineHeight * 3);

    // Draw header line
    SIZE renderedSize = renderer->GetSize();
    RECT rect;
    rect.left = 0;
    rect.right = renderedSize.cx;
    rect.top = 0;
    rect.bottom = 1;
    renderer->DrawRect(rect, grayBrush);

    return renderer->GetScaledSize();
}

int TranslateResultWindow::CreateExpandButton(
    TranslateResultDictionary category,
    int categoryIndex,
    int showedCount,
    int curY,
    Renderer* renderer)
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
            hInstance,
            renderingContext,
            scrollProvider,
            renderingContext->Scale(WindowDescriptor::CreateStretchWindowDescriptor(paddingX * 3, curY)),
            hWindow,
            fontUnderscored,
            Colors::Gray,
            Colors::Black,
            text,
            bind(&TranslateResultWindow::ExpandDictionary, this, categoryIndex));

        AddChildWindow(expandButton);

        renderer->IncreaseHeight(expandButton->GetHeight());
        curY += lineHeight / 2;
    }

    return curY;
}

TranslateResultWindow::~TranslateResultWindow()
{
    DeleteObject(this->fontUnderscored);
}