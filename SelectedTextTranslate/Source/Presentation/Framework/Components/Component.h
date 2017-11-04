#pragma once
#include "Presentation\Framework\Views\View.h"
#include "Presentation\Framework\Components\IComponent.h"

template<class TView>
class Component : public IComponent
{
private:
    vector<SubscriptionDisposer*> subscriptionDisposers;

protected:
    TView* CurrentView;
    LayoutDescriptor Layout;

    void RegisterForDispose(SubscriptionDisposer* disposer);

public:
    Component(TView* view);
    virtual ~Component();

    void SetLayout(LayoutDescriptor layout) override;

    Rect GetBoundingRect() const override;
    Size GetContentSize() const override;
    void Render(bool preserveScrolls = false) const override;

    void Initialize() override;
    void InitializeAndRender(bool preserveScrolls = false) override;

    void MakeVisible() const override;
    void MakeHidden() const override;
};

template <class TView>
void Component<TView>::RegisterForDispose(SubscriptionDisposer* disposer)
{
    subscriptionDisposers.push_back(disposer);
}

template <class TView>
Component<TView>::Component(TView* view)
{
    static_assert(is_base_of<View, TView>::value, "TView must inherit from View");
    CurrentView = view;
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
Size Component<TView>::GetContentSize() const
{
    return CurrentView->GetContentSize();
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
Component<TView>::~Component()
{
    for(auto disposer : subscriptionDisposers)
    {
        delete disposer;
    }
}