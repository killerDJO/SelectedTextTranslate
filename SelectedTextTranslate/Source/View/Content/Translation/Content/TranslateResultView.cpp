#include "View\Content\Translation\Content\TranslateResultView.h"
#include "View\Controls\Buttons\HoverTextButtonWindow.h"

TranslateResultView::TranslateResultView(ViewContext* context, View* parentView, ModelHolder<TranslateResultContent>* modelHolder)
    : ComponentView(context, parentView, modelHolder)
{
}

Size TranslateResultView::RenderContent(Renderer* renderer, TranslateResultContent model)
{
    DestroyChildViews();

    RenderPosition renderPosition = RenderPosition(paddingX, lineHeight);

    vector<TranslateResultCategory> translateCategories = model.GetTranslateResult().GetTranslateCategories();
    for (size_t i = 0; i < translateCategories.size(); ++i)
    {
        TranslateResultCategory category = translateCategories[i];
        vector<TranslateResultCategoryEntry> categoryEntries = category.GetEntries();

        // Draw category header
        renderPosition = renderer->PrintText(category.GetBaseForm(), fontNormal, Colors::Black, renderPosition);

        wstring partOfSpeech = L" - " + wstring(category.GetPartOfSpeech());
        renderPosition = renderer->PrintText(partOfSpeech.c_str(), fontItalic, Colors::Gray, renderPosition.MoveX(1));

        vector<TranslateResultCategoryEntry> showedEntries(0);
        if (model.IsExpanded(i))
        {
            showedEntries = categoryEntries;
        }
        else
        {
            for (size_t j = 0; j < categoryEntries.size(); ++j)
            {
                if (categoryEntries[j].GetScore() < 0.003 && j >= 7)
                {
                    continue;
                }

                showedEntries.push_back(categoryEntries[j]);
            }
        }

        // Draw words
        for (size_t j = 0; j < showedEntries.size(); ++j)
        {
            renderPosition = renderPosition.MoveY(lineHeight).SetX(paddingX * 3);

            TranslateResultCategoryEntry entry = showedEntries[j];
            renderPosition = renderer->PrintText(entry.GetWord(), fontNormal, Colors::Black, renderPosition);

            // Draw reverse translation
            vector<wstring> reverseTranslations = entry.GetReverseTranslations();
            if (reverseTranslations.size() != 0)
            {
                renderPosition = renderer->PrintText(L" - ", fontNormal, Colors::Gray, renderPosition.MoveX(2, scaleProvider));
                for (size_t k = 0; k < reverseTranslations.size(); ++k)
                {
                    wstring text = wstring(reverseTranslations[k]);
                    if (k != reverseTranslations.size() - 1)
                    {
                        text += L", ";
                    }

                    renderPosition = renderer->PrintText(text.c_str(), fontItalic, Colors::Gray, renderPosition);
                }
            }

            int k = entry.GetScore() >= 0.05 ? 0 : (entry.GetScore() >= 0.0025 ? 1 : 2);
            int rateUnit = scaleProvider->Scale(8);
            int strokeHeight = fontNormal->GetStrokeHeight();

            Rect rect = Rect(
                paddingX + k * rateUnit,
                renderPosition.GetY() - strokeHeight + scaleProvider->Scale(2),
                (3 - k) * rateUnit,
                strokeHeight - scaleProvider->Scale(2));

            renderer->DrawRect(rect, lightGrayBrush);
        }

        renderPosition = renderPosition.MoveY(7, scaleProvider).SetX(paddingX * 3);
        renderPosition = CreateExpandButton(model, RenderDescriptor(renderer, renderPosition), category, i, showedEntries.size());

        renderPosition = renderPosition.MoveY(lineHeight).SetX(paddingX);
    }

    renderer->IncreaseWidth(paddingX);
    renderer->IncreaseHeight(paddingY);

    return renderer->GetSize();
}

RenderResult TranslateResultView::CreateExpandButton(
    TranslateResultContent model,
    RenderDescriptor renderDescriptor,
    TranslateResultCategory category,
    int categoryIndex,
    int showedCount)
{
    DWORD hiddenCount = category.GetEntries().size() - showedCount;

    if (model.IsExpanded(categoryIndex) || hiddenCount > 0) {
        wstring text;

        if (!model.IsExpanded(categoryIndex)) {
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

        HoverTextButtonWindow* expandButton = new HoverTextButtonWindow(context, this);
        expandButton->SetPosition(renderDescriptor.GetRenderPosition().GetPosition());
        expandButton->SetFont(fontSmallUnderscored);
        expandButton->SetText(text);
        expandButton->OnClick.Subscribe([categoryIndex, this]()
        {
            OnExpandCategory.Notify(categoryIndex);
        });
        expandButton->InitializeAndRender();

        renderDescriptor.GetRenderer()->UpdateRenderedContentSize(expandButton);
        return RenderResult(expandButton->GetBoundingRect());
    }

    return RenderResult(renderDescriptor.GetRenderPosition());
}