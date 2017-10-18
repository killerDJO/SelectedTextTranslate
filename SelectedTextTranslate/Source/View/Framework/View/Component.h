#pragma once
#include "View\Framework\View\Views\View.h"

template<class TView>
class Component
{
private:
    View* rawView;

protected:
    TView* view;
    ViewContext* context;

    Component(ViewContext* context, TView* view);
    virtual ~Component();

public:
    virtual void SetDescriptor(WindowDescriptor descriptor);

    Rect GetBoundingRect() const;
    void Render(bool preserveScrolls = false) const;
    void Initialize() const;
    void InitializeAndRender(bool preserveScrolls = false) const;

    void MakeVisible() const;
    void MakeHidden() const;

    TView* GetView();
};

template <class TView>
Component<TView>::Component(ViewContext* context, TView* view)
{
    static_assert(is_base_of<View, TView>::value, "TView must inherit from View");
    this->view = view;
    this->rawView = view;
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
    rawView->SetDescriptor(descriptor);
}

template <class TView>
Rect Component<TView>::GetBoundingRect() const
{
    return rawView->GetBoundingRect();
}

template <class TView>
void Component<TView>::Render(bool preserveScrolls) const
{
    rawView->Render(preserveScrolls);
}

template <class TView>
void Component<TView>::Initialize() const
{
    rawView->Initialize();
}

template <class TView>
void Component<TView>::InitializeAndRender(bool preserveScrolls) const
{
    rawView->InitializeAndRender(preserveScrolls);
}

template <class TView>
void Component<TView>::MakeVisible() const
{
    rawView->MakeVisible();
}

template <class TView>
void Component<TView>::MakeHidden() const
{
    rawView->MakeHidden();
}

template <class TView>
TView* Component<TView>::GetView()
{
    return this->view;
}
