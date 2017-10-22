#pragma once
#include "Presentation\Framework\Views\ContentView.h"
#include "Presentation\Framework\Component.h"
#include "Presentation\Framework\ModelHolder.h"

template <class TModel>
class ComponentView : public ContentView
{
protected:
    ModelHolder<TModel>* modelHolder;

    virtual Size RenderContent(Renderer* renderer, TModel model);

    Size RenderContent(Renderer* renderer) override
    {
        return RenderContent(renderer, modelHolder->GetModel());
    }

public:
    ComponentView(CommonContext* context, View* parentView, ModelHolder<TModel>* modelHolder)
        : ContentView(context, parentView)
    {
        this->modelHolder = modelHolder;
    }
};

template <class TModel>
Size ComponentView<TModel>::RenderContent(Renderer* renderer, TModel model)
{
    return this->RenderContent(renderer);
}