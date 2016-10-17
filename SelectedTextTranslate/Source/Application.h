#pragma once
#include "View\Framework\Dto\WindowDescriptor.h"
#include "View\Framework\Providers\ScaleProvider.h"
#include "Infrastructure\Logging\Logger.h"

class Application
{
private:
    vector<void*> components;

    int BootstrapApplication(Logger* logger, HINSTANCE hInstance);
    WindowDescriptor GetMainWindowDescriptor(ScaleProvider* scaleProvider) const;

    template<typename TComponent>
    TComponent* RegisterComponent(TComponent* component);
    void DestroyComponents();

public:
    Application();
    ~Application();

    int Run(HINSTANCE hInstance);
};
