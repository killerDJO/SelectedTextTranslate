#include "Presentation\Components\Translation\Content\TranslateResultView.h"
#include "Presentation\Controls\Buttons\HoverTextButtonControl.h"

TranslateResultView::TranslateResultView(ViewContext* context, View* parentView, ModelHolder<TranslationViewModel*>* modelHolder, IComponent* component)
    : ChildComponentView(context, parentView, modelHolder, component)
{
    this->Name = L"TranslateResultWindow";
}

Size TranslateResultView::RenderContent(Renderer* renderer, TranslationViewModel* model)
{
    DestroyChildViews();

    RenderPosition renderPosition = RenderPosition(PaddingX, LineHeight);

    vector<TranslateResultCategory> translateCategories = model->GetTranslateResult().GetTranslateCategories();
    for (size_t i = 0; i < translateCategories.size(); ++i)
    {
        TranslateResultCategory category = translateCategories[i];
        vector<TranslateResultCategoryEntry> categoryEntries = category.GetEntries();

        // Draw category header
        renderPosition = renderer->PrintText(category.GetBaseForm(), FontNormal, Colors::Black, renderPosition);

        wstring partOfSpeech = L" - " + wstring(category.GetPartOfSpeech());
        renderPosition = renderer->PrintText(partOfSpeech.c_str(), FontItalic, Colors::Gray, renderPosition.MoveX(1));

        vector<TranslateResultCategoryEntry> showedEntries(0);
        if (model->IsExpanded(i))
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
            renderPosition = renderPosition.MoveY(LineHeight).SetX(PaddingX * 3);

            TranslateResultCategoryEntry entry = showedEntries[j];
            renderPosition = renderer->PrintText(entry.GetWord(), FontNormal, Colors::Black, renderPosition);

            // Draw reverse translation
            vector<wstring> reverseTranslations = entry.GetReverseTranslations();
            if (reverseTranslations.size() != 0)
            {
                renderPosition = renderer->PrintText(L" - ", FontNormal, Colors::Gray, renderPosition.MoveX(2, ScaleProvider));
                for (size_t k = 0; k < reverseTranslations.size(); ++k)
                {
                    wstring text = wstring(reverseTranslations[k]);
                    if (k != reverseTranslations.size() - 1)
                    {
                        text += L", ";
                    }

                    renderPosition = renderer->PrintText(text.c_str(), FontItalic, Colors::Gray, renderPosition);
                }
            }

            int k = entry.GetScore() >= 0.05 ? 0 : (entry.GetScore() >= 0.0025 ? 1 : 2);
            int rateUnit = ScaleProvider->Scale(8);
            int strokeHeight = FontNormal->GetStrokeHeight();

            Rect rect = Rect(
                PaddingX + k * rateUnit,
                renderPosition.GetY() - strokeHeight + ScaleProvider->Scale(2),
                (3 - k) * rateUnit,
                strokeHeight - ScaleProvider->Scale(2));

            renderer->DrawRect(rect, LightGrayBrush);
        }

        renderPosition = renderPosition.MoveY(7, ScaleProvider).SetX(PaddingX * 3);
        renderPosition = CreateExpandButton(model, RenderDescriptor(renderer, renderPosition), category, i, showedEntries.size());

        renderPosition = renderPosition.MoveY(LineHeight).SetX(PaddingX);
    }

    renderer->IncreaseWidth(PaddingX);
    renderer->IncreaseHeight(PaddingY);

    return renderer->GetSize();
}

RenderResult TranslateResultView::CreateExpandButton(
    TranslationViewModel* model,
    RenderDescriptor renderDescriptor,
    TranslateResultCategory category,
    int categoryIndex,
    int showedCount)
{
    DWORD hiddenCount = category.GetEntries().size() - showedCount;

    if (model->IsExpanded(categoryIndex) || hiddenCount > 0) {
        wstring text;

        if (!model->IsExpanded(categoryIndex)) {
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

        HoverTextButtonControl* expandButton = new HoverTextButtonControl(Context, this);
        expandButton->SetPosition(renderDescriptor.GetRenderPosition().GetPosition());
        expandButton->SetFont(FontSmallUnderscored);
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