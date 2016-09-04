#include "Windows\Content\DictionaryWindow.h"

DictionaryWindow::DictionaryWindow(AppModel* appModel, HWND parentWindow, HINSTANCE hInstance, DWORD x, DWORD y, DWORD width, DWORD height)
    : ContentWindow(appModel, parentWindow, hInstance, x, y, width, height)
{
}

void DictionaryWindow::ShowFullTranslation(int dictionaryIndex)
{
    appModel->TranslateWordFromDictionary(dictionaryIndex);
}

POINT DictionaryWindow::RenderDC()
{
    ContentWindow::RenderDC();

    vector<LogRecord> records = appModel->GetDictionaryRecords();

    POINT bottomRight = { 0, 0 };

    int curY = paddingY / 2;

    size_t countToShow = min(200, records.size());

    wstring title = L"Showing " + to_wstring(countToShow) + L" out of " + to_wstring(records.size());
    POINT lineBottomRight = PrintText(inMemoryHDC, title.c_str(), fontItalic, colorGray, paddingX, curY, &bottomRight);
    
    curY = lineBottomRight.y + paddingY / 2;
    
    for (size_t i = 0; i < countToShow; ++i)
    {
        LogRecord record = records[i];
        POINT lineBottomRight = PrintText(inMemoryHDC, record.Word.c_str(), fontNormal, colorBlack, paddingX * 2 + 4, curY, &bottomRight);
        PrintText(inMemoryHDC, wstring(L" (" + to_wstring(record.Count) + L")").c_str(), fontNormal, colorGray, lineBottomRight.x + 1, curY, &bottomRight);
        
        HoverIconButtonWindow* translateButton = new HoverIconButtonWindow(
            hWindow,
            hInstance,
            paddingX,
            curY + 2,
            13,
            13,
            IDB_TRANSLATE_INACTIVE,
            IDB_TRANSLATE,
            bind(&DictionaryWindow::ShowFullTranslation, this, i));

        translateButton->Initialize();
        childWindows.push_back(translateButton);

        curY += lineHeight;
    }
    
    return bottomRight;
}

DictionaryWindow::~DictionaryWindow()
{
}