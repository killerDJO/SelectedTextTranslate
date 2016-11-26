#include "View\Content\Dictionary\DictionaryWindow.h"
#include "View\Controls\Buttons\HoverIconButtonWindow.h"

DictionaryWindow::DictionaryWindow(WindowContext* context, Window* parentWindow)
    : ContentWindow(context, parentWindow)
{
    this->OnShowTranslation = Subscribeable<int>();
}

Size DictionaryWindow::RenderContent(Renderer* renderer)
{
    ContentWindow::RenderContent(renderer);
    DestroyChildWindows();

    int normalFontAscent = renderer->GetFontAscent(fontNormal);
    int curY = paddingY / 2 + normalFontAscent;

    size_t countToShow = min(200, model.size());

    wstring title = L"Showing " + to_wstring(countToShow) + L" out of " + to_wstring(model.size());
    renderer->PrintText(title.c_str(), fontItalic, Colors::Gray, Point(paddingX, curY));

    curY += lineHeight;

    for (size_t i = 0; i < countToShow; ++i)
    {
        DictionaryRecord record = model[i];
        Point lineBottomRight = renderer->PrintText(record.GetWord().c_str(), fontNormal, Colors::Black, Point(paddingX * 2 + 4, curY));
        renderer->PrintText(wstring(L" (" + to_wstring(record.GetCount()) + L")").c_str(), fontNormal, Colors::Gray, Point(lineBottomRight.X + 1, curY));

        HoverIconButtonWindow* translateButton = new HoverIconButtonWindow(context, this);
        translateButton->SetDimensions(Point(paddingX, curY + 2 - normalFontAscent), Size(16, 16));
        translateButton->SetNormalIconResource(IDR_TRANSLATE_INACTIVE);
        translateButton->SetHoverIconResource(IDR_TRANSLATE);
        translateButton->OnClick.Subscribe([i, this]() -> void
        {
            return OnShowTranslation.Notify(i);
        });

        AddChildWindow(translateButton);

        curY += lineHeight;
    }

    return renderer->GetScaledSize();
}

DictionaryWindow::~DictionaryWindow()
{
}