#include "View\Content\Dictionary\DictionaryWindow.h"
#include "View\Controls\Buttons\HoverIconButtonWindow.h"

DictionaryWindow::DictionaryWindow(WindowContext* context, WindowDescriptor descriptor, Window* parentWindow, AppController* appController)
    : ContentWindow(context, descriptor, parentWindow, appController)
{
}

void DictionaryWindow::ShowFullTranslation(int dictionaryIndex) const
{
    appController->TranslateWordFromDictionary(dictionaryIndex);
}

Size DictionaryWindow::RenderContent(Renderer* renderer)
{
    ContentWindow::RenderContent(renderer);
    DestroyChildWindows();

    vector<LogRecord> records = appController->GetDictionaryRecords();
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
            this,
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