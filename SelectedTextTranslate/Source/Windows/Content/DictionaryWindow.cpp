#include "Windows\Content\DictionaryWindow.h"

DictionaryWindow::DictionaryWindow(WindowContext* context, WindowDescriptor descriptor, HWND parentWindow, AppModel* appModel)
    : ContentWindow(context, descriptor, parentWindow, appModel)
{
}

void DictionaryWindow::ShowFullTranslation(int dictionaryIndex) const
{
    appModel->TranslateWordFromDictionary(dictionaryIndex);
}

Size DictionaryWindow::RenderDC(Renderer* renderer)
{
    ContentWindow::RenderDC(renderer);
    
    vector<LogRecord> records = appModel->GetDictionaryRecords();
    int normalFontAscent = renderer->GetFontAscent(fontNormal);
    int curY = paddingY / 2 + normalFontAscent;

    size_t countToShow = min(200, records.size());

    wstring title = L"Showing " + to_wstring(countToShow) + L" out of " + to_wstring(records.size());
    renderer->PrintText(title.c_str(), fontItalic, Colors::Gray, Point(paddingX, curY));

    curY += lineHeight;

    for (size_t i = 0; i < countToShow; ++i)
    {
        LogRecord record = records[i];
        Point lineBottomRight = renderer->PrintText(record.Word.c_str(), fontNormal, Colors::Black, Point(paddingX * 2 + 4, curY));
        renderer->PrintText(wstring(L" (" + to_wstring(record.Count) + L")").c_str(), fontNormal, Colors::Gray, Point(lineBottomRight.X + 1, curY));

        HoverIconButtonWindow* translateButton = new HoverIconButtonWindow(
            context,
            WindowDescriptor::CreateFixedWindowDescriptor(Point(paddingX, curY + 2 - normalFontAscent), Size(16, 16)),
            hWindow,
            IDR_TRANSLATE_INACTIVE,
            IDR_TRANSLATE,
            bind(&DictionaryWindow::ShowFullTranslation, this, i));

        AddChildWindow(translateButton);

        curY += lineHeight;
    }

    return renderer->GetScaledSize();
}

DictionaryWindow::~DictionaryWindow()
{
}