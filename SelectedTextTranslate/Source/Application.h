#pragma once
#include "Presentation\Framework\Dto\LayoutDescriptor.h"
#include "Presentation\Framework\Providers\ScaleProvider.h"
#include "Infrastructure\ServiceRegistry\ServiceRegistry.h"

class Application
{
private:
    int BootstrapApplication(ServiceRegistry* serviceRegistry) const;
    LayoutDescriptor GetMainComponentLayout(ScaleProvider* scaleProvider) const;
    void Exit() const;

public:
    Application();

    int Run(HINSTANCE hInstance) const;
};