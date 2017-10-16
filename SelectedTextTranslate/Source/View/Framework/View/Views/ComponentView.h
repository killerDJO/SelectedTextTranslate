#pragma once
#include "View\Framework\View\Views\ContentView.h"
#include "View\Framework\View\Component.h"
#include "View\Framework\ModelHolder.h"

template <class TModel>
class ComponentView : public ContentView
{
protected:
    ModelHolder<TModel>* modelHolder;

    virtual Size RenderContent(Renderer* renderer, TModel model) = 0;

    Size RenderContent(Renderer* renderer) override
    {
        return RenderContent(renderer, modelHolder->GetModel());
    }

public:
    ComponentView(ViewContext* context, View* parentView, ModelHolder<TModel>* modelHolder)
        : ContentView(context, parentView)
    {
        this->modelHolder = modelHolder;
    }
};