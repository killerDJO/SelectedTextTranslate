#pragma once
#include "Infrastructure\ErrorHandling\Exceptions\SelectedTextTranslateFatalException.h"
#include "Utilities\StringUtilities.h"

class CompositionRoot
{
private:
    map<string, void*> serviceRegistry;

    template<typename TService>
    bool IsServiceRegistered() const;

    template<typename TService>
    const char* GetSeriviceName() const;

public:
    template<typename TService>
    TService* GetService();

    template<typename TService>
    void RegisterService(TService* service);

    void DisposeServices();
};

template <typename TService>
bool CompositionRoot::IsServiceRegistered() const
{
    return serviceRegistry.count(typeid(TService).name()) != 0;
}

template <typename TService>
const char* CompositionRoot::GetSeriviceName() const
{
    return typeid(TService).name();
}

template <typename TService>
TService* CompositionRoot::GetService()
{
    const char* serviceName = GetSeriviceName<TService>();
    if (!IsServiceRegistered<TService>())
    {
        throw SelectedTextTranslateFatalException(StringUtilities::Format(L"Service %s is not registered", serviceName));
    }

    return reinterpret_cast<TService>(serviceRegistry[serviceName]);
}

template <typename TService>
void CompositionRoot::RegisterService(TService* service)
{
    const char* serviceName = GetSeriviceName<TService>();
    if (IsServiceRegistered<TService>())
    {
        throw SelectedTextTranslateFatalException(StringUtilities::Format(L"Service %s is already registered", serviceName));
    }

    serviceRegistry[serviceName] = service;
}