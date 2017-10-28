#pragma once
#include "Infrastructure\ServiceRegistry\ServiceRegistry.h"

class ApplicationServiceRegistry
{
private:
    static void RegisterInfrastructure(ServiceRegistry* applicationRegistry);
    static void RegisterDataAccess(ServiceRegistry* applicationRegistry);
    static void RegisterPresentation(ServiceRegistry* applicationRegistry);
    static void RegisterBusinessLogic(ServiceRegistry* applicationRegistry);

public:
    static ServiceRegistry* GetServiceRegistry();
};