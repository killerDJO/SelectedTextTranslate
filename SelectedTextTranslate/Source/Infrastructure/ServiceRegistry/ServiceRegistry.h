#pragma once
#include "Infrastructure\ErrorHandling\Exceptions\SelectedTextTranslateFatalException.h"
#include "Utilities\StringUtilities.h"
#include "Infrastructure\ServiceRegistry\ServiceInfo.h"

class ServiceRegistry
{
private:
    map<string, IServiceInfo*> servicesMap;
    vector<string> dependenciesChain;

    template<typename TService>
    void AssertServiceIsNotRegistered() const;

    template<typename TService>
    void CheckCircularDependencies() const;

protected:
    template<typename TService>
    bool IsServiceRegistered() const;

    template<typename TService>
    const char* GetSeriviceName() const;

public:
    ~ServiceRegistry();

    template<typename TService>
    TService* Get();

    template<typename TService>
    void RegisterSingleton(TService* service, bool ownInstance = true);

    template<typename TService>
    void RegisterSingleton(function<TService*(ServiceRegistry*)> serviceFactory, bool ownInstance = true);

    template<typename TService>
    void Register(function<TService*(ServiceRegistry*)> serviceFactory, InstantiationTypes instantiationType, bool ownInstance = true);

    template<typename TService>
    void Unregister();
};

template <typename TService>
void ServiceRegistry::RegisterSingleton(TService* service, bool ownInstance)
{
    AssertServiceIsNotRegistered<TService>();
    servicesMap[GetSeriviceName<TService>()] = new ServiceInfo<TService>(service, ownInstance);
}

template <typename TService>
void ServiceRegistry::RegisterSingleton(function<TService*(ServiceRegistry*)> serviceFactory, bool ownInstance)
{
    Register<TService>(serviceFactory, Singleton, ownInstance);
}

template <typename TService>
void ServiceRegistry::Register(function<TService*(ServiceRegistry*)> serviceFactory, InstantiationTypes instantiationType, bool ownInstance)
{
    AssertServiceIsNotRegistered<TService>();
    servicesMap[GetSeriviceName<TService>()] = new ServiceInfo<TService>(serviceFactory, instantiationType, ownInstance);
}

template <typename TService>
void ServiceRegistry::Unregister()
{
    const char* serviceName = GetSeriviceName<TService>();
    delete servicesMap[serviceName];
    servicesMap.erase(serviceName);
}

template <typename TService>
TService* ServiceRegistry::Get()
{
    const char* serviceName = GetSeriviceName<TService>();
    if (!IsServiceRegistered<TService>())
    {
        throw SelectedTextTranslateFatalException(StringUtilities::Format(L"%hs is not registered", serviceName));
    }

    CheckCircularDependencies<TService>();

    dependenciesChain.push_back(serviceName);
    TService* instance = reinterpret_cast<TService*>(servicesMap[serviceName]->GetInstance(this));
    dependenciesChain.pop_back();

    return instance;
}

template <typename TService>
bool ServiceRegistry::IsServiceRegistered() const
{
    return servicesMap.count(GetSeriviceName<TService>()) != 0;
}

template <typename TService>
const char* ServiceRegistry::GetSeriviceName() const
{
    return typeid(TService).name();
}

template <typename TService>
void ServiceRegistry::AssertServiceIsNotRegistered() const
{
    const char* serviceName = GetSeriviceName<TService>();
    if (IsServiceRegistered<TService>())
    {
        throw SelectedTextTranslateFatalException(StringUtilities::Format(L"%hs is already registered", serviceName));
    }
}

template <typename TService>
void ServiceRegistry::CheckCircularDependencies() const
{
    const char* serviceName = GetSeriviceName<TService>();
    if (find(dependenciesChain.begin(), dependenciesChain.end(), serviceName) != dependenciesChain.end())
    {
        string trace;
        for (string dependency: dependenciesChain)
        {
            trace += dependency + ", ";
        }
        trace += serviceName;
        throw SelectedTextTranslateFatalException(StringUtilities::Format(L"Dependencies loop is detected. Trace: %hs.", trace.c_str()));
    }
}

inline ServiceRegistry::~ServiceRegistry()
{
    for (auto service : servicesMap)
    {
        delete service.second;
    }
    servicesMap.clear();
}
