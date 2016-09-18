#include "Windows\Content\DictionaryWindow.h"

DictionaryWindow::DictionaryWindow(HINSTANCE hInstance, RenderingContext* renderingContext, ScrollProvider* scrollProvider, WindowDescriptor descriptor, HWND parentWindow, AppModel* appModel)
    : ContentWindow(hInstance, renderingContext, scrollProvider, descriptor, parentWindow, appModel)
{
}

void DictionaryWindow::ShowFullTranslation(int dictionaryIndex)
{
    appModel->TranslateWordFromDictionary(dictionaryIndex);
}

SIZE DictionaryWindow::RenderDC(Renderer* renderer)
{
    ContentWindow::RenderDC(renderer);
    printf("Render\n");
    vector<LogRecord> records = appModel->GetDictionaryRecords();
    int normalFontAscent = renderer->GetFontAscent(fontNormal);
    int curY = paddingY / 2 + normalFontAscent;

    size_t countToShow = min(200, records.size());

    wstring title = L"Showing " + to_wstring(countToShow) + L" out of " + to_wstring(records.size());
    POINT lineBottomRight = renderer->PrintText(title.c_str(), fontItalic, Colors::Gray, paddingX, curY);

    curY += lineHeight;

    for (size_t i = 0; i < countToShow; ++i)
    {
        LogRecord record = records[i];
        POINT lineBottomRight = renderer->PrintText(record.Word.c_str(), fontNormal, Colors::Black, paddingX * 2 + 4, curY);
        renderer->PrintText(wstring(L" (" + to_wstring(record.Count) + L")").c_str(), fontNormal, Colors::Gray, lineBottomRight.x + 1, curY);

        HoverIconButtonWindow* translateButton = new HoverIconButtonWindow(
            hInstance,
            renderingContext,
            scrollProvider,
            renderingContext->Scale(WindowDescriptor::CreateFixedWindowDescriptor(paddingX, curY + 2 - normalFontAscent, 13, 13)),
            hWindow,
            IDB_TRANSLATE_INACTIVE,
            IDB_TRANSLATE,
            bind(&DictionaryWindow::ShowFullTranslation, this, i));

        AddChildWindow(translateButton);

        curY += lineHeight;
    }

    return renderer->GetScaledSize();
}

DictionaryWindow::~DictionaryWindow()
{
}