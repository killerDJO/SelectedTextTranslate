#pragma once

template <class TModel>
class ModelHolder
{
protected:
    TModel model;

public:
    ModelHolder()
    {
    }

    virtual ~ModelHolder()
    {
    }

    virtual void SetModel(TModel model)
    {
        this->model = model;
    }
};