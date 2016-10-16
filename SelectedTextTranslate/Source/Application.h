#pragma once
#include "View\Framework\Dto\WindowDescriptor.h"
#include "View\Framework\Providers\ScaleProvider.h"
#include "Logging\Logger.h"

class Application
{
private:
    int BootstrapApplication(Logger* logger, HINSTANCE hInstance) const;
    WindowDescriptor GetMainWindowDescriptor(ScaleProvider* scaleProvider) const;

public:
    Application();
    ~Application();

    int Run(HINSTANCE hInstance) const;
};