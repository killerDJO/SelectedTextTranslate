#pragma once
#include "Presentation\Framework\Views\ContentView.h"
#include "Presentation\Framework\Component.h"
#include "Presentation\Framework\ModelHolder.h"

template <class TModel>
class ComponentView : public ContentView
{
protected:
    ModelHolder<TModel>* modelHolder;
    TModel GetModel() const;

    virtual Size RenderContent(Renderer* renderer, TModel model);

    Size RenderContent(Renderer* renderer) override;

public:
    ComponentView(CommonContext* context, View* parentView, ModelHolder<TModel>* modelHolder);

    virtual void SetDescriptor(LayoutDescriptor layoutDescriptor);
};

template <class TModel>
ComponentView<TModel>::ComponentView(CommonContext* context, View* parentView, ModelHolder<TModel>* modelHolder)
    : ContentView(context, parentView)
{
    this->modelHolder = modelHolder;
}

template <class TModel>
void ComponentView<TModel>::SetDescriptor(LayoutDescriptor layoutDescriptor)
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