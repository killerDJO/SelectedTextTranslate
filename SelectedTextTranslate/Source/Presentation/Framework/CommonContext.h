#pragma once
#include "Infrastructure\ErrorHandling\ErrorHandler.h"
#include "Infrastructure\ServiceRegistry\ServiceRegistry.h"

class CommonContext
{
private:
    ServiceRegistry* serviceRegistry;

public:
    CommonContext(ServiceRegistry* registry);

    ErrorHandler* GetErrorHandler() const;
    ServiceRegistry* GetServiceRegistry() const;

    template <typename TService>
    TService* Get() const;
};

inline CommonContext::CommonContext(ServiceRegistry* registry)
{
    this->serviceRegistry = registry;
}

inline ErrorHandler* CommonContext::GetErrorHandler() const
{
    return serviceRegistry->Get<ErrorHandler>();
}

inline ServiceRegistry* CommonContext::GetServiceRegistry() const
{
    return serviceRegistry;
}

template <typename TService>
TService* CommonContext::Get() const
{
    return serviceRegistry->Get<TService>();
}
