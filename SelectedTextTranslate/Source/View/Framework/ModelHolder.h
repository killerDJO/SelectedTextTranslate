#pragma once

template <class TModel>
class ModelHolder
{
protected:
    TModel model;

public:
    virtual void SetModel(TModel model)
    {
        this->model = model;
    }

    virtual TModel GetModel()
    {
        return model;
    }
};