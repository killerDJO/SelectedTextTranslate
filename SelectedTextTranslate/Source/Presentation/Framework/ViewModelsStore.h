#pragma once
#include "Infrastructure\ServiceRegistry\ServiceRegistry.h"

class ViewModelsStore : ServiceRegistry
{
public:
    template<typename TViewModel>
    void Store(TViewModel* viewModel);

    template<typename TViewModel>
    TViewModel* Get();

    template<typename TViewModel>
    void EnsureDefault(TViewModel* viewModel);

    template<typename TViewModel>
    void Remove();
};

template <typename TViewModel>
void ViewModelsStore::Store(TViewModel* viewModel)
{
    if (IsServiceRegistered<TViewModel>())
    {
        Unregister<TViewModel>();
    }

    RegisterSingleton(viewModel);
}

template <typename TViewModel>
TViewModel* ViewModelsStore::Get()
{
    return ServiceRegistry::Get<TViewModel>();
}

template <typename TViewModel>
void ViewModelsStore::EnsureDefault(TViewModel* viewModel)
{
    if(!IsServiceRegistered<TViewModel>())
    {
        Store(viewModel);
    }
    else
    {
        delete viewModel;
    }
}

template <typename TViewModel>
void ViewModelsStore::Remove()
{
    Unregister<TViewModel>();
}
