#pragma once
#include "Presentation\Framework\Component.h"
#include "Presentation\Framework\ModelHolder.h"
#include "Presentation\Framework\Views\ChildView.h"

template <class TModel>
class ComponentView : public ChildView
{
protected:
    int PaddingX;
    int PaddingY;
    int LineHeight;

    Font* FontNormal;
    Font* FontHeader;
    Font* FontItalic;
    Font* FontSmall;
    Font* FontSmallUnderscored;

    Brush* GrayBrush;
    Brush* LightGrayBrush;
    Brush* BackgroundBrush;

    ModelHolder<TModel>* CurrentModelHolder;

    TModel GetModel() const;
    virtual Size RenderContent(Renderer* renderer, TModel model);
    Size RenderContent(Renderer* renderer) override;

public:
    ComponentView(CommonContext* context, View* parentView, ModelHolder<TModel>* modelHolder);
    ~ComponentView() override;

    void Render(bool preserveScrolls = false) override;
    virtual void SetLayout(::LayoutDescriptor layoutDescriptor);
};

template <class TModel>
ComponentView<TModel>::ComponentView(CommonContext* context, View* parentView, ModelHolder<TModel>* modelHolder)
    : ChildView(context, parentView)
{
    CurrentModelHolder = modelHolder;

    LineHeight = ScaleProvider->Scale(20);
    PaddingX = ScaleProvider->Scale(15);
    PaddingY = ScaleProvider->Scale(15);

    FontNormal = RenderingProvider->CreateCustomFont(FontSizes::Normal);
    FontHeader = RenderingProvider->CreateCustomFont(FontSizes::Large);
    FontItalic = RenderingProvider->CreateCustomFont(FontSizes::Normal, true);
    FontSmall = RenderingProvider->CreateCustomFont(FontSizes::Small);
    FontSmallUnderscored = RenderingProvider->CreateCustomFont(FontSizes::Small, false, true);

    LightGrayBrush = RenderingProvider->CreateCustomBrush(Colors::LightGray);
    GrayBrush = RenderingProvider->CreateCustomBrush(Colors::Gray);
    BackgroundBrush = RenderingProvider->CreateCustomBrush(Colors::Background);

    ClassName = L"STT_COMPONENT";
}

template <class TModel>
void ComponentView<TModel>::Render(bool preserveScrolls)
{
    bool isStretchWindow = LayoutDescriptor.GetOverflowX() == OverflowModes::Stretch || LayoutDescriptor.GetOverflowY() == OverflowModes::Stretch;
    if (isStretchWindow && !RenderingContext->IsRenderingInProgress())
    {
        ParentView->Render(preserveScrolls);
    }
    else
    {
        ChildView::Render(preserveScrolls);
    }
}

template <class TModel>
void ComponentView<TModel>::SetLayout(::LayoutDescriptor layoutDescriptor)
{
    AssertViewNotInitialized();
    this->LayoutDescriptor = layoutDescriptor;
}

template <class TModel>
TModel ComponentView<TModel>::GetModel() const
{
    return CurrentModelHolder->GetModel();
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
    delete FontNormal;
    delete FontHeader;
    delete FontItalic;
    delete FontSmallUnderscored;
    delete FontSmall;
    delete LightGrayBrush;
    delete GrayBrush;
    delete BackgroundBrush;
}