#pragma once
#include "Presentation\Framework\Component.h"
#include "Presentation\Framework\ModelHolder.h"
#include "Presentation\Framework\Views\ChildView.h"

template <class TModel>
class ComponentView : public ChildView
{
protected:
    int paddingX;
    int paddingY;
    int lineHeight;

    Font* fontNormal;
    Font* fontHeader;
    Font* fontItalic;
    Font* fontSmall;
    Font* fontSmallUnderscored;

    Brush* grayBrush;
    Brush* lightGrayBrush;
    Brush* backgroundBrush;

    ModelHolder<TModel>* modelHolder;

    TModel GetModel() const;
    virtual Size RenderContent(Renderer* renderer, TModel model);
    Size RenderContent(Renderer* renderer) override;

public:
    ComponentView(CommonContext* context, View* parentView, ModelHolder<TModel>* modelHolder);
    ~ComponentView() override;

    void Render(bool preserveScrolls = false) override;
    virtual void SetLayout(LayoutDescriptor layoutDescriptor);
};

template <class TModel>
ComponentView<TModel>::ComponentView(CommonContext* context, View* parentView, ModelHolder<TModel>* modelHolder)
    : ChildView(context, parentView)
{
    this->modelHolder = modelHolder;

    this->lineHeight = scaleProvider->Scale(20);
    this->paddingX = scaleProvider->Scale(15);
    this->paddingY = scaleProvider->Scale(15);

    this->fontNormal = renderingProvider->CreateCustomFont(FontSizes::Normal);
    this->fontHeader = renderingProvider->CreateCustomFont(FontSizes::Large);
    this->fontItalic = renderingProvider->CreateCustomFont(FontSizes::Normal, true);
    this->fontSmall = renderingProvider->CreateCustomFont(FontSizes::Small);
    this->fontSmallUnderscored = renderingProvider->CreateCustomFont(FontSizes::Small, false, true);

    this->lightGrayBrush = renderingProvider->CreateCustomBrush(Colors::LightGray);
    this->grayBrush = renderingProvider->CreateCustomBrush(Colors::Gray);
    this->backgroundBrush = renderingProvider->CreateCustomBrush(Colors::Background);

    this->className = L"STT_COMPONENT";
}

template <class TModel>
void ComponentView<TModel>::Render(bool preserveScrolls)
{
    bool isStretchWindow = layoutDescriptor.GetOverflowX() == OverflowModes::Stretch || layoutDescriptor.GetOverflowY() == OverflowModes::Stretch;
    if (isStretchWindow && !renderingContext->IsRenderingInProgress())
    {
        parentView->Render(preserveScrolls);
    }
    else
    {
        ChildView::Render(preserveScrolls);
    }
}

template <class TModel>
void ComponentView<TModel>::SetLayout(LayoutDescriptor layoutDescriptor)
{
    AssertViewNotInitialized();
    this->layoutDescriptor = layoutDescriptor;
}

template <class TModel>
TModel ComponentView<TModel>::GetModel() const
{
    return modelHolder->GetModel();
}

template <class TModel>
Size ComponentView<TModel>::RenderContent(Renderer* renderer, TModel model)
{
    return this->RenderContent(renderer);
}

template <class TModel>
Size ComponentView<TModel>::RenderContent(Renderer* renderer)
{
    return RenderContent(renderer, GetModel());
}

template <class TModel>
ComponentView<TModel>::~ComponentView()
{
    delete fontNormal;
    delete fontHeader;
    delete fontItalic;
    delete fontSmallUnderscored;
    delete fontSmall;
    delete lightGrayBrush;
    delete grayBrush;
    delete backgroundBrush;
}