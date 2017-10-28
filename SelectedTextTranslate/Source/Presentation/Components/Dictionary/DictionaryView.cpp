#include "Presentation\Components\Dictionary\DictionaryView.h"
#include "Presentation\Controls\Buttons\HoverIconButtonControl.h"
#include "Presentation\Framework\Rendering\Dto\RenderPosition.h"

DictionaryView::DictionaryView(CommonContext* context, View* parentWindow, ModelHolder<vector<DictionaryRecord>>* modelHolder)
    : ComponentView(context, parentWindow, modelHolder)
{
    Name = L"DictionaryWindow";
    iconSize = ScaleProvider->Scale(16);
}

Size DictionaryView::RenderContent(Renderer* renderer, vector<DictionaryRecord> model)
{
    DestroyChildViews();

    RenderPosition renderPosition = RenderPosition(
        PaddingX,
        PaddingY / 2 + FontNormal->GetAscent());

    size_t countToShow = min(200, model.size());

    wstring title = L"Showing " + to_wstring(countToShow) + L" out of " + to_wstring(model.size());
    renderer->PrintText(title, FontItalic, Colors::Gray, renderPosition);

    for (size_t i = 0; i < countToShow; ++i)
    {
        renderPosition = renderPosition.MoveY(LineHeight);

        DictionaryRecord record = model[i];

        renderPosition = renderer->PrintText(
            record.GetWord(),
            FontNormal,
            Colors::Black,
            renderPosition.MoveX(PaddingX));

        renderer->PrintText(L" (" + to_wstring(record.GetCount()) + L")", FontNormal, Colors::Gray, renderPosition);

        renderPosition = renderPosition.SetX(PaddingX);

        HoverIconButtonControl* translateButton = new HoverIconButtonControl(Context, this);
        translateButton->OnClick.Subscribe([i, this] { OnShowTranslation.Notify(i); });
        translateButton
            ->SetDimensions(renderPosition.MoveY(-FontNormal->GetAscent() + 1).GetPosition(), Size(iconSize, iconSize))
            ->SetNormalIconResource(IDR_TRANSLATE_INACTIVE)
            ->SetHoverIconResource(IDR_TRANSLATE)
            ->InitializeAndRender();
    }

    renderer->IncreaseWidth(PaddingX);

    return renderer->GetSize();
}