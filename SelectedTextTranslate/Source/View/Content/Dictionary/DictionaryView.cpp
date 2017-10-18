#include "View\Content\Dictionary\DictionaryView.h"
#include "View\Controls\Buttons\HoverIconButtonWindow.h"
#include "View\Framework\Rendering\Dto\RenderPosition.h"

DictionaryView::DictionaryView(ViewContext* context, View* parentWindow, ModelHolder<vector<DictionaryRecord>>* modelHolder)
    : ComponentView(context, parentWindow, modelHolder)
{
    this->viewName = L"DictionaryWindow";
    this->iconSize = context->Get<ScaleProvider>()->Scale(16);
}

Size DictionaryView::RenderContent(Renderer* renderer, vector<DictionaryRecord> model)
{
    DestroyChildViews();

    RenderPosition renderPosition = RenderPosition(
        paddingX,
        paddingY / 2 + fontNormal->GetAscent());

    size_t countToShow = min(200, model.size());

    wstring title = L"Showing " + to_wstring(countToShow) + L" out of " + to_wstring(model.size());
    renderer->PrintText(title, fontItalic, Colors::Gray, renderPosition);

    for (size_t i = 0; i < countToShow; ++i)
    {
        renderPosition = renderPosition.MoveY(lineHeight);

        DictionaryRecord record = model[i];

        renderPosition = renderer->PrintText(
            record.GetWord(),
            fontNormal,
            Colors::Black,
            renderPosition.MoveX(paddingX));

        renderer->PrintText(L" (" + to_wstring(record.GetCount()) + L")", fontNormal, Colors::Gray, renderPosition);

        renderPosition = renderPosition.SetX(paddingX);

        HoverIconButtonWindow* translateButton = new HoverIconButtonWindow(context, this);
        translateButton->SetDimensions(
            renderPosition.MoveY(-fontNormal->GetAscent() + 1).GetPosition(),
            Size(iconSize, iconSize));
        translateButton->SetNormalIconResource(IDR_TRANSLATE_INACTIVE);
        translateButton->SetHoverIconResource(IDR_TRANSLATE);
        translateButton->OnClick.Subscribe([i, this]()
        {
            return OnShowTranslation.Notify(i);
        });
        translateButton->InitializeAndRender();
    }

    renderer->IncreaseWidth(paddingX);

    return renderer->GetSize();
}