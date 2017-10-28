#pragma once

class ServiceRegistry;

class IServiceInfo
{
public:
    virtual ~IServiceInfo() = default;
    virtual void* GetInstance(ServiceRegistry* registry) = 0;
};