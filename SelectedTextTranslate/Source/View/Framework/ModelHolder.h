#pragma once

template <class TModel>
class ModelHolder
{
public:
    virtual ~ModelHolder() = default;

    virtual TModel GetModel() = 0;
};