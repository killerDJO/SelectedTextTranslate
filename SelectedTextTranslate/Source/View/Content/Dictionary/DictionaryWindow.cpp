#include "View\Content\Dictionary\DictionaryWindow.h"
#include "View\Controls\Buttons\HoverIconButtonWindow.h"
#include "View\Framework\Rendering\Dto\RenderPosition.h"

DictionaryWindow::DictionaryWindow(WindowContext* context, Window* parentWindow)
    : ContentWindow(context, parentWindow)
{
    this->OnShowTranslation = Subscribeable<int>();
}

Size DictionaryWindow::RenderContent(Renderer* renderer)
{
    ContentWindow::RenderContent(renderer);
    DestroyChildWindows();

    int normalFontAscent = context->GetRenderingContext()->GetFontAscent(fontNormal);

    RenderPosition renderPosition = RenderPosition(paddingX, paddingY / 2 + normalFontAscent);

    size_t countToShow = min(200, model.size());

    wstring title = L"Showing " + to_wstring(countToShow) + L" out of " + to_wstring(model.size());
    renderer->PrintText(title.c_str(), fontItalic, Colors::Gray, renderPosition);

    for (size_t i = 0; i < countToShow; ++i)
    {
        renderPosition = renderPosition.MoveY(lineHeight);

        DictionaryRecord record = model[i];
        renderPosition = renderer->PrintText(record.GetWord().c_str(), fontNormal, Colors::Black, renderPosition.MoveX(paddingX));
        renderer->PrintText(wstring(L" (" + to_wstring(record.GetCount()) + L")").c_str(), fontNormal, Colors::Gray, renderPosition);

        renderPosition = renderPosition.SetX(paddingX);

        HoverIconButtonWindow* translateButton = new HoverIconButtonWindow(context, this);
        translateButton->SetDimensions(
            renderPosition.MoveY(- normalFontAscent).MoveY(1, context->GetScaleProvider()).GetPosition(),
            context->GetScaleProvider()->Scale(Size(16, 16)));
        translateButton->SetNormalIconResource(IDR_TRANSLATE_INACTIVE);
        translateButton->SetHoverIconResource(IDR_TRANSLATE);
        translateButton->OnClick.Subscribe([i, this]() -> void
        {
            return OnShowTranslation.Notify(i);
        });

        AddChildWindow(translateButton);
    }

    renderer->IncreaseWidth(paddingX);

    return renderer->GetSize();
}

DictionaryWindow::~DictionaryWindow()
{
}