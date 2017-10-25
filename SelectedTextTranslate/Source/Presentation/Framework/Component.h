#pragma once
#include "Presentation\Framework\Views\View.h"
#include "Presentation\Framework\IComponent.h"

template<class TView>
class Component : public IComponent
{
protected:
    TView* CurrentView;
    CommonContext* Context;
    LayoutDescriptor Layout;

    Component(CommonContext* context, TView* view);
    ~Component() override;

public:
    void SetLayout(LayoutDescriptor layout) override;

    Rect GetBoundingRect() const override;
    void Render(bool preserveScrolls = false) const override;

    void Initialize() override;
    void InitializeAndRender(bool preserveScrolls = false) override;

    void MakeVisible() const override;
    void MakeHidden() const override;
    void Resize() const override;
};

template <class TView>
Component<TView>::Component(CommonContext* context, TView* view)
{
    static_assert(is_base_of<View, TView>::value, "TView must inherit from View");
    CurrentView = view;
    Context = context;
}

template <class TView>
Component<TView>::~Component()
{
    delete CurrentView;
}

template <class TView>
void Component<TView>::SetLayout(LayoutDescriptor layout)
{
    Layout = layout;
    CurrentView->SetLayout(Layout);
}

template <class TView>
Rect Component<TView>::GetBoundingRect() const
{
    return CurrentView->GetBoundingRect();
}

template <class TView>
void Component<TView>::Render(bool preserveScrolls) const
{
    CurrentView->Render(preserveScrolls);
}

template <class TView>
void Component<TView>::Initialize()
{
    CurrentView->Initialize();
}

template <class TView>
void Component<TView>::InitializeAndRender(bool preserveScrolls)
{
    Initialize();
    Render(preserveScrolls);
}

template <class TView>
void Component<TView>::MakeVisible() const
{
    CurrentView->MakeVisible();
}

template <class TView>
void Component<TView>::MakeHidden() const
{
    CurrentView->MakeHidden();
}

template <class TView>
void Component<TView>::Resize() const
{
    CurrentView->Resize();
}