#pragma once
#include "Infrastructure\ServiceRegistry\Enums\InstantiationTypes.h"
#include "Infrastructure\ErrorHandling\Exceptions\SelectedTextTranslateFatalException.h"
#include "Utilities\StringUtilities.h"
#include "Infrastructure\ServiceRegistry\IServiceInfo.h"

class ServiceRegistry;

template<typename TService>
class ServiceInfo : public IServiceInfo
{
private:
    InstantiationTypes instantiationType;
    function<TService*(ServiceRegistry*)> serviceFactory;
    TService* instance;
    bool ownInstance;

public:
    ServiceInfo();
    ServiceInfo(function<TService*(ServiceRegistry*)> serviceFactory, InstantiationTypes instantiationType, bool ownInstance);
    ServiceInfo(TService* instance, bool ownInstance);
    ~ServiceInfo() override;

    void* GetInstance(ServiceRegistry* registry) override;
};

template <typename TService>
ServiceInfo<TService>::ServiceInfo()
{
    instance = nullptr;
    instantiationType = None;
}

template <typename TService>
ServiceInfo<TService>::ServiceInfo(function<TService*(ServiceRegistry*)> serviceFactory, InstantiationTypes instantiationType, bool ownInstance)
{
    this->serviceFactory = serviceFactory;
    this->instantiationType = instantiationType;
    this->ownInstance = ownInstance;
    instance = nullptr;
}

template <typename TService>
ServiceInfo<TService>::ServiceInfo(TService* instance, bool ownInstance)
{
    this->instance = instance;
    this->ownInstance = ownInstance;
    instantiationType = Singleton;
}

template <typename TService>
void* ServiceInfo<TService>::GetInstance(ServiceRegistry* registry)
{
    if (instantiationType == Factory)
    {
        return serviceFactory(registry);
    }

    if (instantiationType == Singleton)
    {
        return instance != nullptr
            ? instance
            : instance = serviceFactory(registry);
    }

    throw SelectedTextTranslateFatalException(StringUtilities::Format(L"Unsupported instantiation type: %d", instantiationType));
}

template <typename TService>
ServiceInfo<TService>::~ServiceInfo()
{
    if (instance != nullptr && ownInstance)
    {
        delete instance;
    }
}
