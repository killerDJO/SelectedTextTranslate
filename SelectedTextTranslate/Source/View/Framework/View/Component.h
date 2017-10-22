#pragma once
#include "View\Framework\View\Views\View.h"
#include "View\Framework\View\IComponent.h"

template<class TView>
class Component : public IComponent
{
protected:
    TView* view;
    CommonContext* context;

    Component(CommonContext* context, TView* view);
    ~Component() override;

public:
    void SetDescriptor(WindowDescriptor descriptor) override;

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
    this->view = view;
    this->context = context;
}

template <class TView>
Component<TView>::~Component()
{
    delete view;
}

template <class TView>
void Component<TView>::SetDescriptor(WindowDescriptor descriptor)
{
    view->SetDescriptor(descriptor);
}

template <class TView>
Rect Component<TView>::GetBoundingRect() const
{
    return view->GetBoundingRect();
}

template <class TView>
void Component<TView>::Render(bool preserveScrolls) const
{
    view->Render(preserveScrolls);
}

template <class TView>
void Component<TView>::Initialize()
{
    view->Initialize();
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
    view->MakeVisible();
}

template <class TView>
void Component<TView>::MakeHidden() const
{
    view->MakeHidden();
}

template <class TView>
void Component<TView>::Resize() const
{
    view->Resize();
}