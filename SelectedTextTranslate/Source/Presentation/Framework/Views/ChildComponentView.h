#pragma once
#include "Presentation\Framework\Components\Component.h"
#include "Presentation\Framework\ModelHolder.h"
#include "Presentation\Framework\Views\ChildView.h"

template <class TModel>
class ChildComponentView : public ChildView
{
private:
    IComponent* component;

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
    ChildComponentView(ViewContext* context, View* parentView, ModelHolder<TModel>* modelHolder, IComponent* component);
    ~ChildComponentView() override;

    void Render(bool preserveScrolls = false) override;
    virtual void SetLayout(::LayoutDescriptor layoutDescriptor);
};

template <class TModel>
ChildComponentView<TModel>::ChildComponentView(ViewContext* context, View* parentView, ModelHolder<TModel>* modelHolder, IComponent* component)
    : ChildView(context, parentView)
{
    CurrentModelHolder = modelHolder;
    this->component = component;

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
void ChildComponentView<TModel>::Render(bool preserveScrolls)
{
    bool isStretchWindow = Layout.GetOverflowX() == OverflowModes::Stretch || Layout.GetOverflowY() == OverflowModes::Stretch;
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
void ChildComponentView<TModel>::SetLayout(LayoutDescriptor layoutDescriptor)
{
    AssertViewNotInitialized();
    this->Layout = layoutDescriptor;
}

template <class TModel>
TModel ChildComponentView<TModel>::GetModel() const
{
    AssertViewInitialized();
    return CurrentModelHolder->GetModel();
}

template <class TModel>
Size ChildComponentView<TModel>::RenderContent(Renderer* renderer, TModel model)
{
    return this->RenderContent(renderer);
}

template <class TModel>
Size ChildComponentView<TModel>::RenderContent(Renderer* renderer)
{
    return RenderContent(renderer, GetModel());
}

template <class TModel>
ChildComponentView<TModel>::~ChildComponentView()
{
    delete component;
    delete FontNormal;
    delete FontHeader;
    delete FontItalic;
    delete FontSmallUnderscored;
    delete FontSmall;
    delete LightGrayBrush;
    delete GrayBrush;
    delete BackgroundBrush;
}